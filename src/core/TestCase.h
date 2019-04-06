#pragma once

#include "MessageTypes.h"

#include <QString>

namespace shakespear
{

struct TestCase
{
    inline static auto type = MessageType::TEST_CASE;
    QString name;
    QString script;
};

QDataStream& operator<<(QDataStream& stream, const TestCase& testCase);
QDataStream& operator>>(QDataStream& stream, TestCase& testCase);

} // namespace shakespear
