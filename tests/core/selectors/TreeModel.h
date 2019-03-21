/** @file
 * @brief      Tree model. Declaration.
 *
 * $Id: $
 */

#pragma once

#include <QAbstractItemModel>

#include <memory>

namespace models
{

class TreeItem;

/**
 * The AbstractTreeItem class provides the model for treeish data.
 * Used in conjunction with TreeItem
 */
class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

    friend class TreeItem;
    friend class InsertRowsGuard;
    friend class RemoveRowsGuard;

public:
    /**
     * Constructs an treeitem model with the given parent.
     */
    explicit TreeModel(QObject* parent = NULL);

    /**
     * Destroys the tree item model.
     */
    virtual ~TreeModel();

    /**
     * Return root item.
     */
    TreeItem* rootItem() const;

    /**
     * Set root item.
     */
    void setRootItem(TreeItem* item);

    /**
     * Returns a pointer to the AbstractTreeItem associated with the given
     * index.
     */
    TreeItem* itemForIndex(const QModelIndex& index) const;

    /**
     * Returns the QModelIndex associated with the given item.
     */
    QModelIndex indexForItem(TreeItem* item) const;

    /**
     * Reimplemented from QAbstractItemModel
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    /**
     * Reimplemented from QAbstractItemModel
     */
    int columnCount(const QModelIndex& parent = QModelIndex()) const;

    /**
     * Reimplemented from QAbstractItemModel
     */
    bool setData(const QModelIndex& index, const QVariant& data, int role);

    /**
     * Reimplemented from QAbstractItemModel
     */
    QVariant data(const QModelIndex& index, int role) const;

    /**
     * Reimplemented from QAbstractItemModel
     */
    QModelIndex
    index(int row, int column, const QModelIndex& index = QModelIndex()) const;

    /**
     * Reimplemented from QAbstractItemModel
     */
    QModelIndex parent(const QModelIndex& index) const;

    /**
     * Reimplemented from QAbstractItemModel
     */
    Qt::ItemFlags flags(const QModelIndex& index) const;

    /**
     * Reimplemented from QAbstractItemModel
     */
    bool hasChildren(const QModelIndex& index) const;

    /**
     * Reimplemented from QAbstractItemModel
     */
    bool canFetchMore(const QModelIndex& index) const;

    /**
     * Reimplemented from QAbstractItemModel
     */
    void fetchMore(const QModelIndex& index);

private:
    std::unique_ptr<TreeItem> m_root;
};

} // namespace models
