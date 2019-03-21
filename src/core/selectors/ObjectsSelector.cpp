#include "ObjectsSelector.h"

#include "ClassMatcher.h"
#include "FuzzyPropertyMatcher.h"
#include "InvalidSelector.h"
#include "LookupError.h"
#include "ObjectNameMatcher.h"
#include "PropertyMatcher.h"
#include "TypeMatcher.h"

#include "log/Log.h"

#include <QAbstractItemModel>
#include <QRegularExpression>

#include <QDebug>

namespace shakespear
{

namespace
{

constexpr auto idKey = "id";
constexpr auto typeKey = "type";
constexpr auto classKey = "class";
constexpr auto propertyKey = "property";
constexpr auto singleLevelKey = "singleLevel";
constexpr auto multiLevelKey = "multiLevel";

class CompoundMatcherBuilder
{
    using matcherBuilder
        = std::function<std::unique_ptr<ObjectMatcher>(const QString&)>;

public:
    CompoundMatcherBuilder() : m_matcher(std::make_unique<CompoundMatcher>())
    {
        m_builders[idKey] = [](const QString& captured) {
            return std::make_unique<ObjectNameMatcher>(captured);
        };
        m_builders[typeKey] = [](const QString& captured) {
            return std::make_unique<TypeMatcher>(captured);
        };
        m_builders[classKey] = [](const QString& captured) {
            return std::make_unique<ClassMatcher>(captured);
        };

        m_builders[propertyKey]
            = [](const QString& captured) -> std::unique_ptr<ObjectMatcher> {
            const QRegularExpression propertyRegExp(
                R"((?<name>\w+)(?<op>~?=)\"(?<value>(\w|\s)+)\")");
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
        if (m_matcher->empty())
        {
            BOOST_THROW_EXCEPTION(exception::EmptyMatcher());
        }
        auto matcher = std::move(m_matcher);
        m_matcher = std::make_unique<CompoundMatcher>();
        return matcher;
    }

    bool empty() const
    {
        return m_matcher->empty();
    }

private:
    std::unique_ptr<CompoundMatcher> m_matcher;
    std::map<QString, matcherBuilder> m_builders;
};

} // namespace

ObjectsSelector::ObjectsSelector(
    const QString& selector, QAbstractItemModel* model, int role)
    : m_model(model), m_role(role), m_matchers(buildMatchers(selector))
{
}

QObject* ObjectsSelector::findObject() const
{
    QModelIndexList indices = { {} };
    for (const auto& matcher: m_matchers)
    {
        indices = findIndices(indices, matcher);
    }

    if (indices.isEmpty())
    {
        BOOST_THROW_EXCEPTION(exception::ObjectNotFound());
    }

    if (indices.size() > 1)
    {
        BOOST_THROW_EXCEPTION(exception::MultipleObjectsFound());
    }

    return indices.at(0).data(m_role).value<QObject*>();
}

QModelIndexList ObjectsSelector::findIndices(
    const QModelIndexList& startIndices, const Matcher& matcher) const
{
    QModelIndexList foundIndices;
    for (const auto& parentIndex: startIndices)
    {
        for (int i = 0; i < m_model->rowCount(parentIndex); ++i)
        {
            auto index = m_model->index(i, 0, parentIndex);
            auto object = index.data(m_role).value<QObject*>();
            if (matcher.matcher->match(*object))
            {
                foundIndices.push_back(index);
            }
            if (matcher.multilevel)
            {
                foundIndices += findIndices({ index }, matcher);
            }
        }
    }
    return foundIndices;
}

std::vector<Matcher> buildMatchers(const QString& selector)
{
    const auto idRegex = QStringLiteral(R"(\#(?<%1>\w+))").arg(idKey);
    const auto classRegex
        = QStringLiteral(R"((?<%1>\w+(:{2}\w+)*))").arg(classKey);
    const auto propertyRegex
        = QStringLiteral(R"(\[(?<%1>\w+~?=\"(\w|\s)+\")\])").arg(propertyKey);
    const auto typeRegex
        = QStringLiteral(R"(\.(?<%1>\w+(:{2}\w+)*))").arg(typeKey);
    const auto singleLevelRegex
        = QStringLiteral(R"((?<%1>>))").arg(singleLevelKey);
    const auto multiLevelRegex
        = QStringLiteral(R"((?<%2>\s))").arg(multiLevelKey);

    const QRegularExpression regExp(QStringLiteral(R"((%1|%2|%3|%4)|%5|%6)")
                                        .arg(
                                            classRegex,
                                            idRegex,
                                            propertyRegex,
                                            typeRegex,
                                            singleLevelRegex,
                                            multiLevelRegex));

    QString capturedString;
    QRegularExpressionMatchIterator i = regExp.globalMatch(selector);
    CompoundMatcherBuilder matcherBuilder;
    std::vector<Matcher> matchers;

    try
    {
        bool multilevel = true;
        while (i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            LOG_DEBUG << match.captured();
            if (!match.captured(multiLevelKey).isEmpty()
                || !match.captured(singleLevelKey).isEmpty())
            {
                // First level modifier can change multilevel property
                if (!matchers.empty() || !matcherBuilder.empty())
                {
                    matchers.push_back({ matcherBuilder.take(), multilevel });
                }
                multilevel = match.captured(singleLevelKey).isEmpty();
            }
            else
            {
                matcherBuilder.addMatch(match);
            }

            capturedString += match.captured();
        }
        matchers.push_back({ matcherBuilder.take(), multilevel });
    }
    catch (const exception::InvalidMatcher&)
    {
        BOOST_THROW_EXCEPTION(::exception::InvalidSelector(selector));
    }

    if (matchers.empty() || capturedString != selector)
    {
        BOOST_THROW_EXCEPTION(::exception::InvalidSelector(selector));
    }

    return matchers;
}

} // namespace shakespear
