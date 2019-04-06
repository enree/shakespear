#include "core/MessageCodec.h"

#include "gqtest"

#include <gmock/gmock.h>

namespace shakespear
{

bool operator==(const CustomMessage& expected, const CustomMessage& actual)
{
    return expected.text == actual.text;
}

class MessageReceiver
{
public:
    MOCK_METHOD1(customMessage, void(CustomMessage));
};

TEST(MessageCodecTest, testCustomMessage)
{
    QString text = "Custom text";
    CustomMessage message = { text };
    MessageCodec codec;
    auto block = codec.encode(message);

    MessageReceiver receiver;
    QObject::connect(
        &codec,
        &MessageCodec::customMessage,
        [&receiver](const CustomMessage& customMessage) {
            receiver.customMessage(customMessage);
        });

    EXPECT_CALL(receiver, customMessage(message)).Times(testing::Exactly(1));

    codec.decode(block);
}

} // namespace shakespear
