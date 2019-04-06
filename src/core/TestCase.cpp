#include "TestCase.h"

#include "gsl/gsl_assert"

namespace shakespear
{

QDataStream& operator<<(QDataStream& stream, const TestCase& testCase)
{
    stream << TestCase::type << testCase.name << testCase.script;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, TestCase& testCase)
{
    MessageType type;
    stream >> type >> testCase.name >> testCase.script;

    Ensures(type == TestCase::type);
    return stream;
}

} // namespace shakespear
