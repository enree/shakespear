#pragma once

#include <QDataStream>

namespace shakespear
{

enum class MessageType : int
{
    TEST_CASE = 0,
    TEST_RESULT = 1,
    MESSAGE = 3
};

inline QDataStream& operator<<(QDataStream& stream, const MessageType& type)
{
    stream << static_cast<int>(type);
    return stream;
}

inline QDataStream& operator>>(QDataStream& stream, MessageType& type)
{
    int _type;
    stream >> _type;
    type = static_cast<MessageType>(_type);
    return stream;
}

} // namespace shakespear
