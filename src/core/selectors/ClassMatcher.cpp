#include "ClassMatcher.h"

#include "gsl/gsl_assert"

namespace shakespear
{

ClassMatcher::ClassMatcher(QString className)
    : ObjectMatcher(true), m_className(std::move(className))
{
    Expects(!m_className.isEmpty());
}

bool ClassMatcher::match(const QObject& object) const
{
    auto metaObject = object.metaObject();
    do
    {
        if (m_className == metaObject->className())
        {
            return true;
        }
        metaObject = metaObject->superClass();
    } while (metaObject != nullptr);

    return false;
}

} // namespace shakespear
