#pragma once

#include "MessageTypes.h"

#include <QStringList>

namespace shakespear
{

struct TestResult
{
    using CodeBase = qint8;
    enum class Code : CodeBase
    {
        PASSED,
        FAILED,
        INVALID_TEST_CASE
    };

    inline static auto type = MessageType::TEST_RESULT;
    QString name;
    Code code;
    QStringList texts = {};
};

QDataStream& operator<<(QDataStream& stream, const TestResult& testResult);
QDataStream& operator>>(QDataStream& stream, TestResult& testResult);

} // namespace shakespear
