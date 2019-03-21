#include "TestObjectTree.h"

#include "TreeItem.h"

#include "gsl/pointers"

namespace shakespear
{

class ObjectItem : public models::TreeItem
{
public:
    explicit ObjectItem(gsl::not_null<QObject*> object) : m_object(object) {}

    QVariant data(int column, int role) const override
    {
        Q_UNUSED(column);
        if (role == Qt::DisplayRole)
        {
            return QString("%1 (%2)").arg(
                m_object->objectName(), m_object->metaObject()->className());
        }

        if (role == TestObjectTree::QObjectRole)
        {
            return QVariant::fromValue(m_object);
        }

        return QVariant();
    }

    ~ObjectItem()
    {
        delete m_object;
    }

private:
    QObject* m_object;
};

template <typename ObjectType>
ObjectItem*
createItem(const QString& name, const std::map<QString, QVariant>& properties)
{
    auto object = new ObjectType;
    object->setObjectName(name);
    for (const auto& property: properties)
    {
        object->setProperty(
            property.first.toLatin1().constData(), property.second);
    }

    return new ObjectItem(object);
}

TestObjectTree::TestObjectTree(QObject* parent) : TreeModel(parent)
{
    auto root = createItem<QObject>("root", {});

    auto firstItem = createItem<QObjectSubclass>("widget", {});
    firstItem->appendChild(
        createItem<
            QObjectSubSubclass>("label", { { "text", QString("label1") } }));
    firstItem->appendChild(createItem<QAlternativeSubSubclass>(
        "eidt",
        { { "text", QString("edit1") },
          { "classes",
            QStringList() << "own"
                          << "shared" } }));

    auto secondItem = createItem<
        QObjectSubSubclass>("label", { { "text", QString("label2") } });
    secondItem->appendChild(createItem<QAlternativeSubSubclass>(
        "edit2",
        { { "text", QString("edit2") },
          { "classes",
            QStringList() << "otherown"
                          << "shared" } }));

    auto thirdItem = createItem<QAlternativeSubSubclass>(
        "edit", { { "text", QString("space beyond space") } });

    root->appendChild(firstItem);
    root->appendChild(secondItem);
    root->appendChild(thirdItem);

    rootItem()->appendChild(root);
    rootItem()->setColumnCount(1);
}

} // namespace shakespear
