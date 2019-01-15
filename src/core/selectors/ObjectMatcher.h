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
    ObjectMatcher(bool unique) : _unique(unique) {}

    virtual bool match(const QObject* const object) const = 0;

    bool unique() const
    {
        return _unique;
    }

private:
    bool _unique;
};

} // namespace shakespear
