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

    bool match(const QObject* const object) const override;

private:
    QString _typeName;
};

} // namespace shakespear
