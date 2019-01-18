#include "FuzzyPropertyMatcher.h"

#include "gsl/gsl_assert"

#include <QVariant>

namespace shakespear
{

FuzzyPropertyMatcher::FuzzyPropertyMatcher(QString name, QString value)
    : ObjectMatcher(false), m_name(std::move(name)), m_value(std::move(value))
{
    Expects(!m_name.isEmpty());
    Expects(!m_value.isEmpty());
}

bool FuzzyPropertyMatcher::match(const QObject& object) const
{
    QVariant property = object.property(m_name.toLatin1().constData());
    if (property.canConvert<QStringList>())
    {
        QStringList list = property.toStringList();
        return list.contains(m_value);
    }
    return false;
}

} // namespace shakespear
