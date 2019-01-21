/**
 * @file
 * @brief tree item
 *
 * @ingroup RIO_CORIOLIS
 *
 * @copyright  (C) 2015 PKB RIO Design Department
 *
 * $Id: $
 *
**/

#include "TreeItem.h"
#include "TreeModel.h"

#include "utils/AsConst.h"

namespace rio
{

namespace models
{

/**
 * Helper class. Call beginInsertRows/endInsertRows if model is not NULL
 * These classes SHALL NOT be in anonimous namespace because they are friends of TreeModel
 */
class InsertRowsGuard
{
public:
    InsertRowsGuard(TreeModel* model, const QModelIndex& parent, int from, int to)
        : m_model(model)
    {
        if (m_model != nullptr)
        {
            m_model->beginInsertRows(parent, from, to);
        }
    }

    ~InsertRowsGuard()
    {
        if (m_model != nullptr)
        {
            m_model->endInsertRows();
        }
    }

private:
    TreeModel* m_model;
};

/**
 * Helper class. Call beginRemoveRows/endRemoveRows if model is not NULL
 */
class RemoveRowsGuard
{
public:
    RemoveRowsGuard(TreeModel* model, const QModelIndex& parent, int from, int to)
        : m_model(model)
    {
        if (m_model != nullptr)
        {
            m_model->beginRemoveRows(parent, from, to);
        }
    }

    ~RemoveRowsGuard()
    {
        if (m_model != nullptr)
        {
            m_model->endRemoveRows();
        }
    }

private:
    TreeModel* m_model;
};

TreeItem::TreeItem(TreeItem* parent)
    : m_parent(parent)
    , m_model(nullptr)
    , m_flags(Qt::NoItemFlags)
    , m_columnCount(0)
{
}

TreeItem::~TreeItem()
{
    clear();
}

TreeItem* TreeItem::parent() const
{
    return m_parent;
}

void TreeItem::setParent(TreeItem* parent)
{
    m_parent = parent;
}

TreeModel* TreeItem::model() const
{
    return m_parent != nullptr ? m_parent->model() : m_model;
}

void TreeItem::setModel(TreeModel* model)
{
    Q_ASSERT(model);
    m_model = model;
}

int TreeItem::rowCount() const
{
    return m_children.size();
}

int TreeItem::columnCount() const
{
    return m_parent != nullptr ? m_parent->columnCount() : m_columnCount;
}

void TreeItem::setColumnCount(int columnCount)
{
    m_columnCount = columnCount;
    update();
}

QVariant TreeItem::data(int column, int role) const
{
    Q_UNUSED(column);
    Q_UNUSED(role);
    return QVariant();
}

bool TreeItem::setData(int column, const QVariant& data, int role)
{
    Q_UNUSED(column);
    Q_UNUSED(role);
    Q_UNUSED(data);
    return false;
}

Qt::ItemFlags TreeItem::flags(int column) const
{
    Q_UNUSED(column);
    return m_flags;
}

void TreeItem::setFlags(Qt::ItemFlags flags)
{
    m_flags = flags;
}

void TreeItem::update()
{
    if (TreeModel* itemModel = model())
    {
        QModelIndex idx = index();
        emit itemModel->dataChanged(idx.sibling(idx.row(), 0),
            idx.sibling(idx.row(), itemModel->columnCount() - 1));
    }

    for (auto child: utils::asConst(m_children))
    {
        child->update();
    }
}

void TreeItem::updateColumn(int column)
{
    if (TreeModel* itemModel = model())
    {
        QModelIndex idx = index();
        emit itemModel->dataChanged(idx.sibling(idx.row(), column),
                             idx.sibling(idx.row(), column));
    }
}

QModelIndex TreeItem::index() const
{
    TreeModel* itemModel = model();
    return itemModel != nullptr
        ? itemModel->indexForItem(const_cast<TreeItem*>(this)) : QModelIndex(); // NOLINT
}

bool TreeItem::canFetchMore() const
{
    return false;
}

void TreeItem::fetchMore()
{
    // Do nothing
}

bool TreeItem::hasChildren() const
{
    return canFetchMore() || rowCount() > 0;
}

TreeItem* TreeItem::child(int row) const
{
    Q_ASSERT(row >= 0 && row < m_children.size());
    return m_children.at(row);
}

int TreeItem::indexOf(TreeItem* item) const
{
    return m_children.indexOf(item);
}

void TreeItem::appendChild(TreeItem* item)
{
    insertChild(m_children.size(), item);
}

void TreeItem::appendChildren(const QList<TreeItem*>& items)
{
    insertChildren(m_children.size(), items);
}

void TreeItem::insertChild(int row, TreeItem* item)
{
    insertChildren(row, QList<TreeItem*>() << item);
}

void TreeItem::insertChildren(int row, const QList<TreeItem*>& items)
{
    if(items.isEmpty())
    {
        return;
    }

    if (0 <= row && row <= m_children.size())
    {
        InsertRowsGuard guard(model(), index(), row, row + items.count() - 1);

        QList<TreeItem*>::iterator it = m_children.begin() + row;
        for (auto item: items)
        {
            item->setParent(this);
            it = m_children.insert(it, item) + 1;
        }

    }
}

void TreeItem::removeChild(TreeItem* child)
{
    if (child == nullptr || rowCount() == 0)
    {
        return;
    }

    int row = m_children.indexOf(child);
    if (row >= 0)
    {
        removeChildren(row, row);
    }
}

void TreeItem::removeChild(int row)
{
    removeChildren(row, row);
}

void TreeItem::removeChildren(int from, int to)
{
    Q_ASSERT(from <= to);
    Q_ASSERT(from >= 0);
    Q_ASSERT(to < rowCount());

    RemoveRowsGuard guard(model(), index(), from, to);
    QList<TreeItem*>::iterator first = m_children.begin() + from;
    QList<TreeItem*>::iterator last = m_children.begin() + to + 1;

    qDeleteAll(first, last);
    m_children.erase(first, last);
}

void TreeItem::removeChildren()
{
    TreeModel* itemModel = model();

    if (rowCount() == 0)
    {
        return;
    }

    if (itemModel != nullptr)
    {
        QModelIndex idx = index();
        itemModel->beginRemoveRows(idx, 0, rowCount() - 1);
        clear();
        itemModel->endRemoveRows();
    }
    else
    {
        clear();
    }
}

void TreeItem::clear()
{
    qDeleteAll(m_children);
    m_children.clear();
}

} // models

} // rio
