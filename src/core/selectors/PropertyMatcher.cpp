#include "PropertyMatcher.h"

#include "gsl/gsl_assert"

#include <QVariant>

namespace shakespear
{

PropertyMatcher::PropertyMatcher(QString property, QString value)
    : ObjectMatcher(false)
    , m_property(std::move(property))
    , m_value(std::move(value))
{
    Expects(!m_property.isEmpty());
    Expects(!m_value.isEmpty());
}

bool PropertyMatcher::match(const QObject& object) const
{
    return object.property(m_property.toLatin1().constData()).toString()
           == m_value;
}

} // namespace shakespear
