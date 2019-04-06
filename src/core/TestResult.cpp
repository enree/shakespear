#include "TestResult.h"

#include "gsl/gsl_assert"

namespace shakespear
{

namespace
{

QDataStream& operator<<(QDataStream& stream, const TestResult::Code& code)
{
    stream << static_cast<TestResult::CodeBase>(code);
    return stream;
}

inline QDataStream& operator>>(QDataStream& stream, TestResult::Code& code)
{
    TestResult::CodeBase _code;
    stream >> _code;
    code = static_cast<TestResult::Code>(_code);
    return stream;
}

} // namespace

QDataStream& operator<<(QDataStream& stream, const TestResult& testResult)
{
    stream << TestResult::type << testResult.name << testResult.code
           << testResult.texts;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, TestResult& testResult)
{
    MessageType type;
    stream >> type >> testResult.name >> testResult.code >> testResult.texts;

    Ensures(type == TestResult::type);
    return stream;
}

} // namespace shakespear
