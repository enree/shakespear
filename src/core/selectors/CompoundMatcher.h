#pragma once

#include "ObjectMatcher.h"

#include <memory>
#include <vector>

namespace shakespear
{

/**
 * This matcher check QObject over several matchers
 */
class CompoundMatcher : public ObjectMatcher
{
public:
    CompoundMatcher();
    /**
     * Add specific @a matcher
     */
    void add(std::unique_ptr<ObjectMatcher> matcher);

    bool match(const QObject& object) const override;

private:
    std::vector<std::unique_ptr<ObjectMatcher> > m_matchers;
};

} // namespace shakespear
