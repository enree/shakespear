#pragma once

#include "CompoundMatcher.h"

#include <QModelIndexList>

#include <memory>
#include <vector>

class QAbstractItemModel;
class QString;

namespace shakespear
{

struct Matcher
{
    std::unique_ptr<CompoundMatcher> matcher;
    bool multilevel;
};

class ObjectsSelector
{

public:
    ObjectsSelector(
        const QString& selector, QAbstractItemModel* model, int role);

    /**
     * Find one object. Throws an exception if no object or multiple object are
     * found
     */
    QObject* findObject() const;

private:
    QModelIndexList findIndices(
        const QModelIndexList& startIndices, const Matcher& matcher) const;

private:
    QAbstractItemModel* m_model;
    int m_role;

    std::vector<Matcher> m_matchers;
};

std::vector<Matcher> buildMatchers(const QString& selector);

} // namespace shakespear
