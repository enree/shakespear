#pragma once

#include "ObjectMatcher.h"

namespace shakespear
{

/**
 * This matcher check QObject name
 */
class ObjectNameMatcher : public ObjectMatcher
{
public:
    explicit ObjectNameMatcher(QString objectName);

    bool match(const QObject& object) const override;

private:
    QString m_objectName;
};

} // namespace shakespear
