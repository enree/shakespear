#include "ObjectNameMatcher.h"

namespace shakespear
{

ObjectNameMatcher::ObjectNameMatcher(QString objectName)
    : ObjectMatcher(true), _objectName(std::move(objectName))
{
}

bool ObjectNameMatcher::match(const QObject* const object) const
{
    return object->objectName() == _objectName;
}

} // namespace shakespear
