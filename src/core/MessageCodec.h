#pragma once

#include "CustomMessage.h"
#include "TestCase.h"
#include "TestResult.h"

#include <QDataStream>
#include <QObject>

namespace shakespear
{

constexpr auto dataVersion = QDataStream::Qt_5_12;

class MessageCodec : public QObject
{
    Q_OBJECT
public:
    explicit MessageCodec(QObject* parent = nullptr);

signals:
    void testCase(TestCase);
    void testResult(TestResult);
    void customMessage(CustomMessage);

public:
    template <typename T>
    QByteArray encode(const T& message)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(dataVersion);
        out << message;
        return block;
    }

public slots:
    void decode(QByteArray block);
};

} // namespace shakespear
