/** @file
 * @brief      Common tree model. Implementation.
 *
 * @ingroup RIO_CORIOLIS_
 *
 * @copyright  (C) 2015 PKB RIO Design Department
 *
 * $Id: $
 */

#include "TreeModel.h"

#include "TreeItem.h"

namespace rio
{

namespace models
{

TreeModel::TreeModel(QObject* parent)
    : QAbstractItemModel(parent),
      m_root(new TreeItem)
{
    m_root->setModel(this);
}

TreeModel::~TreeModel() = default;

TreeItem* TreeModel::rootItem() const
{
    return m_root.get();
}

void TreeModel::setRootItem(TreeItem* item)
{
    Q_ASSERT(item != nullptr);
    Q_ASSERT(item->model() == nullptr);
    Q_ASSERT(item->parent() == nullptr);

    emit layoutAboutToBeChanged();
    m_root.reset(item);
    item->setModel(this);
    emit layoutChanged();
}

TreeItem *TreeModel::itemForIndex(const QModelIndex & index) const
{
    TreeItem* item = index.isValid() ?
                static_cast<TreeItem*>(index.internalPointer()) : m_root.get();
    Q_ASSERT(item);
    Q_ASSERT(item->model());
    return item;
}

QModelIndex TreeModel::indexForItem(TreeItem *item) const
{
    Q_ASSERT(item);

    if (item == m_root.get())
    {
        return {};
    }

    TreeItem* parent = item->parent();
    Q_ASSERT(parent);

    int row = parent->indexOf(item);
    return createIndex(row, 0, item);
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &index) const
{
    if (!hasIndex(row, column, index))
    {
        return {};
    }

    const TreeItem *item = itemForIndex(index);
    Q_ASSERT(item);

    if (row >= item->rowCount())
    {
        return {};
    }

    return createIndex(row, column, (void*)(item->child(row)));
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return {};
    }

    const TreeItem *item = itemForIndex(index);
    Q_ASSERT(item);

    TreeItem *parent = item->parent();
    if (parent == nullptr || parent == m_root.get())
    {
        return {};
    }

    const TreeItem *grandparent = parent->parent();
    if (grandparent == nullptr)
    {
        return {};
    }

    for (int i = 0, n = grandparent->rowCount(); i < n; ++i)
    {
        if (grandparent->child(i) == parent)
        {
            return createIndex(i, 0, (void*) parent);
        }
    }

    return {};
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    TreeItem *item = itemForIndex(index);
    return item != nullptr ? item->flags(index.column())
                : (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

bool TreeModel::hasChildren(const QModelIndex &index) const
{
    TreeItem *item = itemForIndex(index);
    return item == nullptr || item->hasChildren(); // Is the first condition true?
}

bool TreeModel::canFetchMore(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return false;
    }

    TreeItem *item = itemForIndex(index);
    return item != nullptr ? item->canFetchMore() : false;
}

void TreeModel::fetchMore(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }

    if (TreeItem *item = itemForIndex(index))
    {
        item->fetchMore();
    }
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return m_root->rowCount();
    }

    if (parent.column() > 0)
    {
        return 0;
    }

    const TreeItem *item = itemForIndex(parent);
    Q_ASSERT(item);
    return item->rowCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return m_root->columnCount();
    }

    const TreeItem *item = itemForIndex(parent);
    Q_ASSERT(item);
    return item->columnCount();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &data, int role)
{
    TreeItem *item = itemForIndex(index);
    bool res = item != nullptr ? item->setData(index.column(), data, role) : false;
    if (res)
    {
        emit dataChanged(index, index);
    }
    return res;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    TreeItem *item = itemForIndex(index);
    return item != nullptr ? item->data(index.column(), role) : QVariant();
}

} // models

} // rio
