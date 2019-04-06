#pragma once

#include "MessageTypes.h"

#include <QDataStream>

namespace shakespear
{

struct CustomMessage
{
    inline static auto type = MessageType::MESSAGE;
    QString text;
};

QDataStream& operator<<(QDataStream& stream, const CustomMessage& message);
QDataStream& operator>>(QDataStream& stream, CustomMessage& message);

} // namespace shakespear
