#pragma once

#include "ObjectMatcher.h"

namespace shakespear
{

/**
 * This matcher check only class name.
 * className SHOUD contain all namespaces
 */
class TypeMatcher : public ObjectMatcher
{
public:
    explicit TypeMatcher(QString typeName);

    bool match(const QObject& object) const override;

private:
    QString m_typeName;
};

} // namespace shakespear
