#include "core/MessageCodec.h"

#include "gqtest"

#include <gmock/gmock.h>

namespace shakespear
{

bool operator==(const CustomMessage& expected, const CustomMessage& actual)
{
    return expected.text == actual.text;
}

bool operator==(const TestCase& expected, const TestCase& actual)
{
    return std::tie(expected.name, expected.script)
           == std::tie(actual.name, actual.script);
}

bool operator==(const TestResult& expected, const TestResult& actual)
{
    return std::tie(expected.name, expected.code, expected.texts)
           == std::tie(actual.name, actual.code, actual.texts);
}

class MessageReceiver
{
public:
    MOCK_METHOD1(customMessage, void(CustomMessage));
    MOCK_METHOD1(testCase, void(TestCase));
    MOCK_METHOD1(testResult, void(TestResult));
};

TEST(MessageCodecTest, testCustomMessage)
{
    CustomMessage message = { "Custom text" };
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

TEST(MessageCodecTest, testTestCase)
{
    TestCase testCase = { "name", "script" };
    MessageCodec codec;
    auto block = codec.encode(testCase);

    MessageReceiver receiver;
    QObject::connect(
        &codec, &MessageCodec::testCase, [&receiver](const TestCase& testCase) {
            receiver.testCase(testCase);
        });

    EXPECT_CALL(receiver, testCase(testCase)).Times(testing::Exactly(1));

    codec.decode(block);
}

TEST(MessageCodecTest, testTestResult)
{
    TestResult testResult = { "name",
                              TestResult::Code::PASSED,
                              QStringList() << "textOne"
                                            << "textTwo" };
    MessageCodec codec;
    auto block = codec.encode(testResult);

    MessageReceiver receiver;
    QObject::connect(
        &codec,
        &MessageCodec::testResult,
        [&receiver](const TestResult& testResult) {
            receiver.testResult(testResult);
        });

    EXPECT_CALL(receiver, testResult(testResult)).Times(testing::Exactly(1));

    codec.decode(block);
}

} // namespace shakespear
