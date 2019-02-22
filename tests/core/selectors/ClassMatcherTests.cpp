#include "DummyObjects.h"

#include "core/selectors/ClassMatcher.h"

#include "gqtest"

namespace shakespear
{

TEST(ClassMatcherTest, unique)
{
    EXPECT_TRUE(ClassMatcher("shakespear::QObjectSubclass").unique());
}

TEST(ClassMatcherTest, selfMatch)
{
    QObjectSubclass object;
    EXPECT_TRUE(ClassMatcher("shakespear::QObjectSubclass").match(object));
}

TEST(ClassMatcherTest, parentMatch)
{
    QObjectSubSubclass object;
    EXPECT_TRUE(ClassMatcher("shakespear::QObjectSubclass").match(object));
}

TEST(ClassMatcherTest, grandParentMatch)
{
    QObjectSubSubclass object;
    EXPECT_TRUE(ClassMatcher("QObject").match(object));
}

TEST(ClassMatcherTest, childDoNotMatch)
{
    QObjectSubclass object;
    EXPECT_FALSE(ClassMatcher("shakespear::QObjectSubSubclass").match(object));
}

TEST(ClassMatcherTest, unrelatedDoNotMatch)
{
    QObjectSubSubclass object;
    EXPECT_FALSE(ClassMatcher("QEventLoop").match(object));
}

} // namespace shakespear
