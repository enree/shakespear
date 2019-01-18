#include "PropertyMatcher.h"

#include "gsl/gsl_assert"

#include <QVariant>

namespace shakespear
{

PropertyMatcher::PropertyMatcher(QString name, QString value)
    : ObjectMatcher(false), m_name(std::move(name)), m_value(std::move(value))
{
    Expects(!m_name.isEmpty());
    Expects(!m_value.isEmpty());
}

bool PropertyMatcher::match(const QObject& object) const
{
    return object.property(m_name.toLatin1().constData()).toString() == m_value;
}

} // namespace shakespear
