#pragma once

#include "ObjectMatcher.h"

namespace shakespear
{

/**
 * This matcher match QObject againt property
 */
class PropertyMatcher : public ObjectMatcher
{
public:
    explicit PropertyMatcher(QString property, QString value);

    bool match(const QObject& object) const override;

private:
    QString m_property;
    QString m_value;
};

} // namespace shakespear
