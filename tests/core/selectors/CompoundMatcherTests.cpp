#include "DummyObjects.h"

#include "core/selectors/CompoundMatcher.h"
#include "core/selectors/ClassMatcher.h"
#include "core/selectors/InvalidSelector.h"
#include "core/selectors/PropertyMatcher.h"
#include "core/selectors/ObjectNameMatcher.h"

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
        exception::InvalidSelector);
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

} // namespace shakespear
