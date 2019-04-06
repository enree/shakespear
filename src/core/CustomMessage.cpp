#include "CustomMessage.h"

#include "gsl/gsl_assert"

namespace shakespear
{

QDataStream& operator<<(QDataStream& stream, const CustomMessage& message)
{
    stream << CustomMessage::type << message.text;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, CustomMessage& message)
{
    MessageType type;
    stream >> type >> message.text;

    Ensures(type == CustomMessage::type);
    return stream;
}

} // namespace shakespear
