#pragma once

#include "ObjectMatcher.h"

namespace shakespear
{

/**
 * This matcher check class name and all it's superclasses.
 * className SHOUD contain all namespaces
 */
class ClassMatcher : public ObjectMatcher
{
public:
    explicit ClassMatcher(QString className);

    bool match(const QObject& object) const override;

private:
    QString m_className;
};

} // namespace shakespear
