#include "DummyObjects.h"

#include "core/selectors/ClassMatcher.h"
#include "core/selectors/CompoundMatcher.h"
#include "core/selectors/InvalidSelector.h"
#include "core/selectors/ObjectNameMatcher.h"
#include "core/selectors/ObjectsSelector.h"
#include "core/selectors/PropertyMatcher.h"

#include "gqtest"

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
    EXPECT_TRUE(buildMatchers("#name").front().matcher->match(object));
}

TEST(CompoundMatcherFromSelectorTest, classMatcher)
{
    QObjectSubSubclass object;
    EXPECT_TRUE(buildMatchers("shakespear::QObjectSubclass")
                    .front()
                    .matcher->match(object));
}

TEST(CompoundMatcherFromSelectorTest, typeMatcher)
{
    QObjectSubclass object;
    EXPECT_TRUE(buildMatchers(".shakespear::QObjectSubclass")
                    .front()
                    .matcher->match(object));
}

TEST(CompoundMatcherFromSelectorTest, propertyMatcher)
{
    QObject object;
    object.setProperty("Property", "value");
    EXPECT_TRUE(
        buildMatchers(R"([Property="value"])").front().matcher->match(object));
}

TEST(CompoundMatcherFromSelectorTest, propertyWithSpaceMatcher)
{
    QObject object;
    object.setProperty("Property", "value and space");
    EXPECT_TRUE(buildMatchers(R"([Property="value and space"])")
                    .front()
                    .matcher->match(object));
}

TEST(CompoundMatcherFromSelectorTest, twoPropertiesMatcher)
{
    QObject object;
    object.setProperty("Property1", "value1");
    object.setProperty("Property2", "value2");
    EXPECT_TRUE(buildMatchers(R"([Property1="value1"][Property2="value2"])")
                    .front()
                    .matcher->match(object));
}

TEST(CompoundMatcherFromSelectorTest, fuzzyPropertyMather)
{
    QObject object;
    object.setProperty(
        "Property",
        QStringList() << "value1"
                      << "value2");
    EXPECT_TRUE(buildMatchers(R"([Property~="value1"])")
                    .front()
                    .matcher->match(object));
}

TEST(CompoundMatcherFromSelectorTest, allFieldsMatcher)
{
    QObjectSubclass object;
    object.setProperty("Property", "value");
    object.setObjectName("name");

    EXPECT_TRUE(buildMatchers(R"(QObject#name[Property="value"])")
                    .front()
                    .matcher->match(object));
    EXPECT_TRUE(buildMatchers(R"([Property="value"]QObject#name)")
                    .front()
                    .matcher->match(object));
    EXPECT_TRUE(buildMatchers(R"(#name[Property="value"]QObject)")
                    .front()
                    .matcher->match(object));
}

TEST(CompoundMatcherFromSelectorTest, allFieldsMatcherDoNotMatch)
{
    QObjectSubclass object;
    object.setProperty("Property", "value");

    EXPECT_FALSE(buildMatchers(R"(QObject#name[Property="value"])")
                     .front()
                     .matcher->match(object));
}

TEST(CompoundMatcherFromSelectorTest, invalidSelectors)
{
    EXPECT_THROW(
        buildMatchers(R"([property="value")"), exception::InvalidSelector);

    EXPECT_THROW(
        buildMatchers(R"(property="value"])"), exception::InvalidSelector);

    EXPECT_THROW(
        buildMatchers(R"([property=."value"])"), exception::InvalidSelector);

    EXPECT_THROW(
        buildMatchers(R"([property=value])"), exception::InvalidSelector);

    EXPECT_THROW(buildMatchers(R"(#id#name)"), exception::InvalidSelector);

    EXPECT_THROW(buildMatchers(R"(##)"), exception::InvalidSelector);

    EXPECT_THROW(buildMatchers(R"(###)"), exception::InvalidSelector);

    EXPECT_THROW(buildMatchers(R"(.class#)"), exception::InvalidSelector);

    EXPECT_THROW(buildMatchers(R"(.clas#$)"), exception::InvalidSelector);

    EXPECT_THROW(buildMatchers(R"(namespace::)"), exception::InvalidSelector);

    EXPECT_THROW(
        buildMatchers(R"(namespace::Widget::)"), exception::InvalidSelector);

    EXPECT_THROW(buildMatchers(R"(::)"), exception::InvalidSelector);
}

} // namespace shakespear
