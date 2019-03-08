#include "CompoundMatcher.h"

#include "ClassMatcher.h"
#include "FuzzyPropertyMatcher.h"
#include "InvalidSelector.h"
#include "ObjectNameMatcher.h"
#include "PropertyMatcher.h"
#include "TypeMatcher.h"

#include <QRegularExpression>

#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/type_index.hpp>

#include <iostream>

namespace shakespear
{

namespace
{

class DuplicateUniqueMatcher : public exception::General
{
};

class CompoundMatcherBuilder
{
    using matcherBuilder
        = std::function<std::unique_ptr<ObjectMatcher>(const QString&)>;

public:
    CompoundMatcherBuilder() : m_matcher(std::make_unique<CompoundMatcher>())
    {
        m_builders["id"] = [this](const QString& captured) {
            m_matchedParts += ("#" + captured);
            return std::make_unique<ObjectNameMatcher>(captured);
        };
        m_builders["type"] = [this](const QString& captured) {
            m_matchedParts += ("." + captured);
            return std::make_unique<TypeMatcher>(captured);
        };
        m_builders["class"] = [this](const QString& captured) {
            m_matchedParts += captured;
            return std::make_unique<ClassMatcher>(captured);
        };

        m_builders["property"] =
            [this](const QString& captured) -> std::unique_ptr<ObjectMatcher> {
            m_matchedParts += ("[" + captured + "]");

            const QRegularExpression propertyRegExp(
                R"((?<name>\w+)(?<op>~?=)\"(?<value>\w+)\")");
            QRegularExpressionMatch propertyMatch
                = propertyRegExp.match(captured);

            const QString propertyName = propertyMatch.captured("name");
            const QString propertyValue = propertyMatch.captured("value");
            const QString eqOp = propertyMatch.captured("op");

            if (eqOp == "=")
            {
                return std::make_unique<
                    PropertyMatcher>(propertyName, propertyValue);
            }

            return std::make_unique<
                FuzzyPropertyMatcher>(propertyName, propertyValue);
        };
    }

    void addMatch(const QRegularExpressionMatch& match)
    {
        for (const auto& builder: m_builders)
        {
            QString matchedString = match.captured(builder.first);
            if (!matchedString.isEmpty())
            {
                m_matcher->add(builder.second(matchedString));
            }
        }
    }

    std::unique_ptr<CompoundMatcher> take()
    {
        return std::move(m_matcher);
    }

    QString processedSelector() const
    {
        return m_matchedParts;
    }

private:
    std::unique_ptr<CompoundMatcher> m_matcher;
    std::map<QString, matcherBuilder> m_builders;

    QString m_matchedParts;
};

} // namespace

CompoundMatcher::CompoundMatcher() : ObjectMatcher(false) {}

void CompoundMatcher::add(std::unique_ptr<ObjectMatcher> matcher)
{
    if (matcher->unique())
    {
        for (const auto& matcherInVector: m_matchers)
        {
            if (boost::typeindex::type_id_runtime(*matcherInVector)
                == boost::typeindex::type_id_runtime(*matcher))
            {
                BOOST_THROW_EXCEPTION(DuplicateUniqueMatcher());
            }
        }
    }

    m_matchers.push_back(std::move(matcher));
}

bool CompoundMatcher::match(const QObject& object) const
{
    return !m_matchers.empty()
           && boost::algorithm::
                  all_of(m_matchers, [&object](const auto& matcher) {
                      return matcher->match(object);
                  });
}

std::unique_ptr<CompoundMatcher> buildMatcher(const QString& selector)
{
    const QRegularExpression regExp(
        R"((?<class>^\w+(:{2}\w+)*)|\#(?<id>\w+)|\[(?<property>\w+~?=\"\w+\")\]|\.(?<type>\w+(:{2}\w+)*))");

    CompoundMatcherBuilder matcherBuilder;

    QRegularExpressionMatchIterator i = regExp.globalMatch(selector);
    while (i.hasNext())
    {
        try
        {
            matcherBuilder.addMatch(i.next());
        }
        catch (const DuplicateUniqueMatcher&)
        {
            BOOST_THROW_EXCEPTION(exception::InvalidSelector(selector));
        }
    }

    //     Selector has unrecognized parts
    if (matcherBuilder.processedSelector() != selector)
    {
        BOOST_THROW_EXCEPTION(exception::InvalidSelector(selector));
    }
    return matcherBuilder.take();
}

} // namespace shakespear
