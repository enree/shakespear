#pragma once

#include <QObject>

namespace shakespear
{

/**
 * Base class to match object to some rule
 */
class ObjectMatcher
{
public:
    ObjectMatcher(bool unique) : m_unique(unique) {}

    virtual bool match(const QObject& object) const = 0;

    bool unique() const
    {
        return m_unique;
    }

    virtual ~ObjectMatcher() = default;

private:
    bool m_unique;
};

} // namespace shakespear
