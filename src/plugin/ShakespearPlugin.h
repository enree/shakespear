#pragma once

#include "log/Log.h"

#include "core/TestCase.h"

#include <QObject>
#include <QTimer>

#include <memory>

class QJSEngine;

namespace GammaRay
{

class Probe;

}

namespace shakespear
{

class GammarayObjectSelector;
class NetworkClient;
class MessageCodec;

/**
 * Host part of Shakespear
 */
class ShakespearPlugin : public QObject
{
    Q_OBJECT
public:
    explicit ShakespearPlugin(GammaRay::Probe* probe, QObject* parent = 0);

    ~ShakespearPlugin();

private slots:
    void initialize();
    void executeTestCase(const shakespear::TestCase& testCase);

private:
    void importModule(const QString& name);
    void evaluate(const TestCase& testCase);

private:
    std::unique_ptr<QJSEngine> m_engine;

    QTimer m_startupTimer;

    std::unique_ptr<NetworkClient> m_client;
    std::unique_ptr<MessageCodec> m_codec;

    std::unique_ptr<appkit::logger::Log> m_logger;
};

} // namespace shakespear
