#include "core/selectors/ObjectNameMatcher.h"

#include "gtest/gtest.h"

#include <QObject>

namespace shakespear
{

TEST(ObjectNameMatcherTest, unique)
{
    EXPECT_TRUE(ObjectNameMatcher("shakespear::QObjectSubclass").unique());
}

TEST(ObjectNameMatcherTest, nameMatch)
{
    QObject object;
    object.setObjectName("name");

    EXPECT_TRUE(ObjectNameMatcher("name").match(object));
}

TEST(ObjectNameMatcherTest, nameDoNotMatch)
{
    QObject object;
    object.setObjectName("name");

    EXPECT_FALSE(ObjectNameMatcher("name1").match(object));
}

} // namespace shakespear
