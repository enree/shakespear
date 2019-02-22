#include "core/selectors/FuzzyPropertyMatcher.h"

#include "gqtest"

#include <QObject>
#include <QStringList>
#include <QVariant>

namespace shakespear
{

TEST(FuzzyPropertyMatcherTest, notUnique)
{
    EXPECT_FALSE(FuzzyPropertyMatcher("Property", "value").unique());
}

TEST(FuzzyPropertyMatcherTest, propertyContainsStringMatch)
{
    QObject object;
    object.setProperty(
        "Property",
        QStringList() << "value1"
                      << "value2");
    EXPECT_TRUE(FuzzyPropertyMatcher("Property", "value2").match(object));
}

TEST(FuzzyPropertyMatcherTest, propertyNotContainsStringDoNotMatch)
{
    QObject object;
    object.setProperty(
        "Property",
        QStringList() << "value1"
                      << "value2");
    EXPECT_FALSE(FuzzyPropertyMatcher("Property", "value").match(object));
}

TEST(FuzzyPropertyMatcherTest, simpleStringContainsPropertyDoNotMatch)
{
    QObject object;
    object.setProperty("Property", "value1 value2");
    EXPECT_FALSE(FuzzyPropertyMatcher("Property", "value").match(object));
}

} // namespace shakespear
