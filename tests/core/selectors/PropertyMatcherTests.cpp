#include "core/selectors/PropertyMatcher.h"

#include "gqtest"

#include <QObject>
#include <QSize>
#include <QVariant>

namespace shakespear
{

TEST(PropertyMatcherTest, notUnique)
{
    EXPECT_FALSE(PropertyMatcher("Property", "value").unique());
}

TEST(PropertyMatcherTest, propertyCaseMatch)
{
    QObject object;
    object.setProperty("Property", "value");
    EXPECT_TRUE(PropertyMatcher("Property", "value").match(object));
}

TEST(PropertyMatcherTest, propertyCaseDoNotMatch)
{
    QObject object;
    object.setProperty("Property", "Value");
    EXPECT_FALSE(PropertyMatcher("property", "value").match(object));
}

TEST(PropertyMatcherTest, noPropertyDoNotMatch)
{
    QObject object;
    EXPECT_FALSE(PropertyMatcher("Property", "value").match(object));
}

TEST(PropertyMatcherTest, propertyConvertableToQStringMatch)
{
    QObject object;
    object.setProperty("Property", 10);
    EXPECT_TRUE(PropertyMatcher("Property", "10").match(object));
}

TEST(PropertyMatcherTest, propertyNotConvertableToQStringDoNotMatch)
{
    QObject object;
    object.setProperty("Propery", QSize(10, 10));
    EXPECT_FALSE(PropertyMatcher("Property", "10x10").match(object));
}

} // namespace shakespear
