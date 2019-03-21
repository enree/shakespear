#include "TestObjectTree.h"

#include "core/selectors/InvalidSelector.h"
#include "core/selectors/LookupError.h"
#include "core/selectors/ObjectsSelector.h"

#include "gqtest"

namespace shakespear
{

TEST(ObjectsSelectorTest, selectRoot)
{
    TestObjectTree objectTree;

    ObjectsSelector selector("#root", &objectTree, TestObjectTree::QObjectRole);

    auto object = selector.findObject();
    EXPECT_EQ("root", object->objectName());
}

TEST(ObjectsSelectorTest, selectOnlyFirstLevel)
{
    TestObjectTree objectTree;

    ObjectsSelector
        selector(">#widget", &objectTree, TestObjectTree::QObjectRole);

    EXPECT_THROW(selector.findObject(), exception::LookupError);
}

TEST(ObjectsSelectorTest, selectNonRootByType)
{
    TestObjectTree objectTree;

    ObjectsSelector selector(
        ".shakespear::QObjectSubclass",
        &objectTree,
        TestObjectTree::QObjectRole);

    auto object = selector.findObject();
    EXPECT_EQ("widget", object->objectName());
}

TEST(ObjectsSelectorTest, selectByChildType)
{
    TestObjectTree objectTree;

    ObjectsSelector
        selector("#root>#label", &objectTree, TestObjectTree::QObjectRole);

    auto object = selector.findObject();
    EXPECT_EQ("label2", object->property("text").toString());
}

TEST(ObjectsSelectorTest, selectByMultipleLevels)
{
    TestObjectTree objectTree;

    ObjectsSelector
        selector("#root #label", &objectTree, TestObjectTree::QObjectRole);

    EXPECT_THROW(selector.findObject(), exception::MultipleObjectsFound);
}

TEST(ObjectsSelectorTest, selectByOneOfProperties)
{
    TestObjectTree objectTree;

    ObjectsSelector selector(
        "#label>[classes~=\"shared\"]",
        &objectTree,
        TestObjectTree::QObjectRole);

    auto object = selector.findObject();
    EXPECT_EQ("edit2", object->objectName());
}

TEST(ObjectsSelectorTest, propertyWithSpaceSelector)
{
    TestObjectTree objectTree;

    ObjectsSelector selector(
        "[text=\"space beyond space\"]",
        &objectTree,
        TestObjectTree::QObjectRole);

    auto object = selector.findObject();
    EXPECT_EQ("edit", object->objectName());
}

TEST(ObjectsSelectorTest, invalidSelectors)
{
    TestObjectTree objectTree;

    EXPECT_THROW(
        ObjectsSelector("#root#label", &objectTree, TestObjectTree::QObjectRole),
        exception::InvalidSelector);

    EXPECT_THROW(
        ObjectsSelector("#root>", &objectTree, TestObjectTree::QObjectRole),
        exception::InvalidSelector);

    EXPECT_THROW(
        ObjectsSelector(
            "#root>>#label", &objectTree, TestObjectTree::QObjectRole),
        exception::InvalidSelector);

    EXPECT_THROW(
        ObjectsSelector(
            "#root> #label", &objectTree, TestObjectTree::QObjectRole),
        exception::InvalidSelector);
}

TEST(ObjectsSelectorTest, findFirst)
{
    TestObjectTree objectTree;

    ObjectsSelector
        selector("#root #label", &objectTree, TestObjectTree::QObjectRole);

    auto object = selector.findFirstObject();
    EXPECT_EQ("label", object->objectName());
}

TEST(ObjectsSelectorTest, findAll)
{
    TestObjectTree objectTree;

    ObjectsSelector
        selector("#root #label", &objectTree, TestObjectTree::QObjectRole);

    auto objects = selector.findObjects();
    ASSERT_EQ(2UL, objects.size());
}

} // namespace shakespear
