#include "TypeMatcher.h"

#include "gsl/gsl_assert"

namespace shakespear
{

TypeMatcher::TypeMatcher(QString typeName)
    : ObjectMatcher(true), m_typeName(std::move(typeName))
{
    Expects(!m_typeName.isEmpty());
}

bool TypeMatcher::match(const QObject& object) const
{
    return object.metaObject()->className() == m_typeName;
}

} // namespace shakespear
