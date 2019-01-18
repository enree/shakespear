#include "CompoundMatcher.h"

#include "ClassMatcher.h"
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

class DuplicateUniqueMatcher : public rio::exception::General
{
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
        R"(((^\w+(:{2}\w+)*))|(\#\w+)|(\[\w+\~?=\"\w+\"\])|(\.\w+(:{2}\w+)*))");
    auto compoundMatcher = std::make_unique<CompoundMatcher>();

    QString allMatchedParts;
    QRegularExpressionMatchIterator i = regExp.globalMatch(selector);
    while (i.hasNext())
    {
        const QRegularExpressionMatch match = i.next();
        const QString matched = match.captured();

        try
        {
            if (matched.startsWith("#"))
            {
                compoundMatcher->add(
                    std::make_unique<ObjectNameMatcher>(matched.mid(1)));
            }
            else if (matched.startsWith("."))
            {
                compoundMatcher->add(
                    std::make_unique<TypeMatcher>(matched.mid(1)));
            }
            else if (matched.startsWith("["))
            {
                const QRegularExpression propertyRegExp(
                    R"(\[(\w+)(=)\"(\w+)\"\])");
                QRegularExpressionMatch propertyMatch
                    = propertyRegExp.match(matched);

                const QString propertyName = propertyMatch.captured(1);
                const QString propertyValue = propertyMatch.captured(3);
                const QString eqOp = propertyMatch.captured(2);

                if (eqOp == "=")
                {
                    compoundMatcher->add(
                        std::make_unique<
                            PropertyMatcher>(propertyName, propertyValue));
                }
            }
            else
            {
                compoundMatcher->add(std::make_unique<ClassMatcher>(matched));
            }
        }
        catch (const DuplicateUniqueMatcher&)
        {
            BOOST_THROW_EXCEPTION(exception::InvalidSelector(selector));
        }

        allMatchedParts += matched;
    }

    // Selector has unrecognized parts
    if (allMatchedParts != selector)
    {
        BOOST_THROW_EXCEPTION(exception::InvalidSelector(selector));
    }
    return compoundMatcher;
}

} // namespace shakespear
