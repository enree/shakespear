#include "DummyObjects.h"

#include "core/selectors/TypeMatcher.h"

#include "gtest/gtest.h"

namespace shakespear
{

TEST(TypeMatcherTest, unique)
{
    EXPECT_TRUE(TypeMatcher("shakespear::QObjectSubclass").unique());
}

TEST(TypeMatcherTest, selfMatch)
{
    QObjectSubclass object;
    EXPECT_TRUE(TypeMatcher("shakespear::QObjectSubclass").match(object));
}

TEST(TypeMatcherTest, parentDoNotMatch)
{
    QObjectSubclass object;
    EXPECT_FALSE(TypeMatcher("QObject").match(object));
}

TEST(TypeMatcherTest, childDoNotMatch)
{
    QObject object;
    EXPECT_FALSE(TypeMatcher("shakespear::QObjectSubclass").match(object));
}

TEST(TypeMatcherTest, unrelatedDoNotMatch)
{
    QObject object;
    EXPECT_FALSE(TypeMatcher("QEventLoop").match(object));
}

} // namespace shakespear
