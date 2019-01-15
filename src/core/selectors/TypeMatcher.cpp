#include "TypeMatcher.h"

namespace shakespear
{

TypeMatcher::TypeMatcher(QString typeName)
    : ObjectMatcher(true), _typeName(std::move(typeName))
{
}

bool TypeMatcher::match(const QObject* const object) const
{
    return object->metaObject()->className() == _typeName;
}

} // namespace shakespear
