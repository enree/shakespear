#include "ObjectNameMatcher.h"

#include "gsl/gsl_assert"

namespace shakespear
{

ObjectNameMatcher::ObjectNameMatcher(QString objectName)
    : ObjectMatcher(true), m_objectName(std::move(objectName))
{
    Expects(!m_objectName.isEmpty());
}

bool ObjectNameMatcher::match(const QObject& object) const
{
    return object.objectName() == m_objectName;
}

} // namespace shakespear
