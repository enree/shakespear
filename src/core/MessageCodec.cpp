#include "MessageCodec.h"

#include "log/Log.h"
#include "qt/Strings.h"

namespace shakespear
{

MessageCodec::MessageCodec(QObject* parent) : QObject(parent) {}

void MessageCodec::decode(QByteArray block)
{
    QDataStream out(&block, QIODevice::ReadOnly);
    out.setVersion(dataVersion);

    MessageType type;
    out >> type;
    out.device()->seek(0);

    switch (type)
    {
    case MessageType::MESSAGE:
    {
        CustomMessage message;
        out >> message;
        emit customMessage(message);
        break;
    }
    case MessageType::TEST_CASE:
    {
        TestCase _testCase;
        out >> _testCase;
        emit testCase(_testCase);
        break;
    }
    default:;
        // Do nothing
    }
}

} // namespace shakespear
