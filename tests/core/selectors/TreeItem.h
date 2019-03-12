/**
 * @file
 * @brief CommonTreeItem
 *
 * $Id: $
 *
 **/

#pragma once

#include <QAbstractItemModel>

namespace models
{

class TreeModel;

/**
 * TreeItem class provides an interface of item used in tree construction.
 * Used in conjunction with TreeModel class.
 */
class TreeItem
{
public:
    /**
     * Constructs item.
     */
    explicit TreeItem(TreeItem* parent = NULL);

    /**
     * Destructor. Causes the item's children to be destroyed as well.
     */
    virtual ~TreeItem();

    /**
     * Returns the item's parent item, or NULL if the item has no parent.
     */
    TreeItem* parent() const;

    /**
     * Set parent for item
     */
    void setParent(TreeItem* parent);

    /**
     * Returns the item's model, or NULL if the item has no model.
     */
    TreeModel* model() const;

    /**
     * Set model for item and all its children
     */
    void setModel(TreeModel* model);

    /**
     * Returns the number of child item rows that the item has.
     */
    virtual int rowCount() const;

    /**
     * Returns the number of columns
     */
    virtual int columnCount() const;

    /**
     * Set the number of column. Need use with root item in AbstractTreeModel
     */
    void setColumnCount(int columnCount);

    /**
     * Returns the item's data for the given role and column,
     * or an invalid QVariant if there is no data for the role.
     */
    virtual QVariant data(int column, int role) const;

    /**
     * Sets the item's data for the given role and column to the specified
     * value.
     */
    virtual bool setData(int column, const QVariant& data, int role);

    /**
     * Returns the item flags for the given column.
     */
    virtual Qt::ItemFlags flags(int column) const;

    /**
     * Sets the item flags for the item to @a flags.
     */
    void setFlags(Qt::ItemFlags flags);

    /**
     * Emit signal AbstractTreeModel::dataChanged for current index and update
     * all children.
     */
    virtual void update();

    /**
     * Emit signal AbstractTreeModel::dataChanged for given column.
     */
    void updateColumn(int column);

    /**
     * Get model index
     */
    QModelIndex index() const;

    /**
     * Returns true if there is more data available for parent; otherwise
     * returns false. The default implementation always returns false.
     */
    virtual bool canFetchMore() const;

    /**
     * Fetch any available data for the items with the parent specified by the
     * parent index. Reimplement this if you are populating your model
     * incrementally. The default implementation does nothing.
     */
    virtual void fetchMore();

    /**
     * Returns true if this item has any children, otherwise returns false.
     */
    bool hasChildren() const;

    /**
     * Returns the child item at row if exist, otherwise returns NULL.
     */
    TreeItem* child(int row) const;

    /**
     * Returns index of item if it exists or -1 otherwise.
     */
    int indexOf(TreeItem* item) const;

    /**
     * Append child @a item.
     */
    void appendChild(TreeItem* item);

    /**
     * Append @a items to children.
     */
    void appendChildren(const QList<TreeItem*>& items);

    /**
     * Insert child @a item in specified @a row.
     */
    void insertChild(int row, TreeItem* item);

    /**
     * Insert a list of @a items in specified @a row
     */
    void insertChildren(int row, const QList<TreeItem*>& items);

    /**
     * Remove the given child and its children.
     */
    void removeChild(TreeItem* child);

    /**
     * Remove child by index.
     */
    void removeChild(int row);

    /**
     * Remove children from @a from row up to @a to row
     */
    void removeChildren(int from, int to);

    /**
     * Remove all children.
     */
    void removeChildren();

private:
    void clear();

private:
    TreeItem* m_parent;
    TreeModel* m_model;
    Qt::ItemFlags m_flags;
    int m_columnCount;
    QList<TreeItem*> m_children;
};

} // namespace models
