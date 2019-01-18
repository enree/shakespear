#include "DummyObjects.h"

#include "core/selectors/ClassMatcher.h"
#include "core/selectors/CompoundMatcher.h"
#include "core/selectors/InvalidSelector.h"
#include "core/selectors/ObjectNameMatcher.h"
#include "core/selectors/PropertyMatcher.h"

#include "gtest/gtest.h"

#include <QVariant>

namespace shakespear
{

TEST(CompoundMatcherTest, addUniqueMatcherOfSameTypeShouldThrow)
{
    CompoundMatcher matcher;
    matcher.add(std::make_unique<ObjectNameMatcher>("id00"));
    EXPECT_THROW(
        matcher.add(std::make_unique<ObjectNameMatcher>("id01")),
        std::exception);
}

TEST(CompoundMatcherTest, twoPropertyMatchersMatches)
{
    CompoundMatcher matcher;
    matcher.add(std::make_unique<PropertyMatcher>("stringProperty", "string"));
    matcher.add(std::make_unique<PropertyMatcher>("intProperty", "100"));

    QObject object;
    object.setProperty("stringProperty", "string");
    object.setProperty("intProperty", 100);

    EXPECT_TRUE(matcher.match(object));
}

TEST(CompoundMatcherTest, oneOfPropertyMatchersDoNotMatch)
{
    CompoundMatcher matcher;
    matcher.add(std::make_unique<PropertyMatcher>("stringProperty", "string"));
    matcher.add(std::make_unique<PropertyMatcher>("intProperty", "100"));

    QObject object;
    object.setProperty("stringProperty", "string");
    object.setProperty("intProperty", 101);

    EXPECT_FALSE(matcher.match(object));
}

TEST(CompoundMatcherTest, classAndNameMatch)
{
    CompoundMatcher matcher;
    matcher.add(std::make_unique<ClassMatcher>("shakespear::QObjectSubclass"));
    matcher.add(std::make_unique<ObjectNameMatcher>("id"));

    QObjectSubSubclass object;
    object.setObjectName("id");
    object.setProperty("intProperty", 101);

    EXPECT_TRUE(matcher.match(object));
}

TEST(CompoundMatcherFromSelectorTest, objectNameMatcher)
{
    QObject object;
    object.setObjectName("name");
    EXPECT_TRUE(buildMatcher("#name")->match(object));
}

TEST(CompoundMatcherFromSelectorTest, classMatcher)
{
    QObjectSubSubclass object;
    EXPECT_TRUE(buildMatcher("shakespear::QObjectSubclass")->match(object));
}

TEST(CompoundMatcherFromSelectorTest, typeMatcher)
{
    QObjectSubclass object;
    EXPECT_TRUE(buildMatcher(".shakespear::QObjectSubclass")->match(object));
}

TEST(CompoundMatcherFromSelectorTest, propertyMatcher)
{
    QObject object;
    object.setProperty("Property", "value");
    EXPECT_TRUE(buildMatcher(R"([Property="value"])")->match(object));
}

TEST(CompoundMatcherFromSelectorTest, twoPropertiesMatcher)
{
    QObject object;
    object.setProperty("Property1", "value1");
    object.setProperty("Property2", "value2");
    EXPECT_TRUE(buildMatcher(R"([Property1="value1"][Property2="value2"])")
                    ->match(object));
}

// TEST(CompoundMatcherFromSelectorTest, twoUniqueSelectorFailed) {}

TEST(CompoundMatcherFromSelectorTest, allFieldsMatcher)
{
    QObjectSubclass object;
    object.setProperty("Property", "value");
    object.setObjectName("name");

    EXPECT_TRUE(
        buildMatcher(R"(QObject#name[Property="value"])")->match(object));
}

TEST(CompoundMatcherFromSelectorTest, allFieldsMatcherDoNotMatch)
{
    QObjectSubclass object;
    object.setProperty("Property", "value");

    EXPECT_FALSE(
        buildMatcher(R"(QObject#name[Property="value"])")->match(object));
}

TEST(CompoundMatcherFromSelectorTest, invalidSelectors)
{
    EXPECT_THROW(
        buildMatcher(R"([property="value")"), exception::InvalidSelector);

    EXPECT_THROW(
        buildMatcher(R"(property="value"])"), exception::InvalidSelector);

    EXPECT_THROW(
        buildMatcher(R"([property=."value"])"), exception::InvalidSelector);

    EXPECT_THROW(
        buildMatcher(R"([property=value])"), exception::InvalidSelector);

    EXPECT_THROW(buildMatcher(R"(#id#name)"), exception::InvalidSelector);

    EXPECT_THROW(buildMatcher(R"(##)"), exception::InvalidSelector);

    EXPECT_THROW(buildMatcher(R"(###)"), exception::InvalidSelector);

    EXPECT_THROW(
        buildMatcher(R"([property="100"]QLineEdit.QWidget#id)"),
        exception::InvalidSelector);

    EXPECT_THROW(buildMatcher(R"(.class#)"), exception::InvalidSelector);

    EXPECT_THROW(buildMatcher(R"(.clas#$)"), exception::InvalidSelector);

    EXPECT_THROW(buildMatcher(R"(namespace::)"), exception::InvalidSelector);

    EXPECT_THROW(
        buildMatcher(R"(namespace::Widget::)"), exception::InvalidSelector);

    EXPECT_THROW(buildMatcher(R"(::)"), exception::InvalidSelector);
}

} // namespace shakespear
