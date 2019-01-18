#pragma once

#include "ObjectMatcher.h"

namespace shakespear
{

/**
 * Match QObject if property @a name is a QStringList and
 * contains value in it
 */
class FuzzyPropertyMatcher : public ObjectMatcher
{
public:
    explicit FuzzyPropertyMatcher(QString name, QString value);

    bool match(const QObject& object) const override;

private:
    QString m_name;
    QString m_value;
};

} // namespace shakespear
