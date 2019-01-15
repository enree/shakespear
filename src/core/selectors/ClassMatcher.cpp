#include "ClassMatcher.h"

namespace shakespear
{

ClassMatcher::ClassMatcher(QString className)
    : ObjectMatcher(true), _className(std::move(className))
{
}

bool ClassMatcher::match(const QObject* const object) const
{
    auto metaObject = object->metaObject();
    do
    {
        if (_className == metaObject->className())
        {
            return true;
        }
        metaObject = metaObject->superClass();
    } while (metaObject != nullptr);

    return false;
}

} // namespace shakespear
