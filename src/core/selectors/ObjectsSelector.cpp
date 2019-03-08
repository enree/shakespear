#include "ObjectsSelector.h"

#include "InvalidSelector.h"
#include "LookupError.h"

#include <QAbstractItemModel>
#include <QRegularExpression>

namespace shakespear
{

ObjectsSelector::ObjectsSelector(
    const QString& selector, QAbstractItemModel* model, int role)
    : m_model(model), m_role(role)
{
    const QRegularExpression regExp(
        R"((?<multilevel>[\s>])?(?<selector>[^\s^>]+)+)");

    QString capturedString;
    QRegularExpressionMatchIterator i = regExp.globalMatch(selector);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        m_matchers.push_back({ buildMatcher(match.captured("selector")),
                               match.captured("multilevel") != ">" });
        capturedString += match.captured();
    }

    if (m_matchers.empty() || capturedString != selector)
    {
        BOOST_THROW_EXCEPTION(::exception::InvalidSelector(selector));
    }
}

QObject* ObjectsSelector::findObject() const
{
    QModelIndexList indices = { {} };
    for (const auto& matcher: m_matchers)
    {
        indices = findIndices(indices, *matcher.matcher, matcher.multilevel);
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
    const QModelIndexList& startIndices,
    const CompoundMatcher& matcher,
    bool multilevel) const
{
    QModelIndexList foundIndices;
    for (const auto& parentIndex: startIndices)
    {
        for (int i = 0; i < m_model->rowCount(parentIndex); ++i)
        {
            auto index = m_model->index(i, 0, parentIndex);
            auto object = index.data(m_role).value<QObject*>();
            if (matcher.match(*object))
            {
                foundIndices.push_back(index);
            }
            if (multilevel)
            {
                foundIndices += findIndices({ index }, matcher, multilevel);
            }
        }
    }
    return foundIndices;
}

} // namespace shakespear
