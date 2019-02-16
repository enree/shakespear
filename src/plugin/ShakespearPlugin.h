#pragma once

#include "shakespear/Log.h"

#include <QDataStream>
#include <QObject>
#include <QTimer>

#include <memory>

class QScriptEngine;
class QTcpServer;

namespace GammaRay
{

class Probe;

}

namespace shakespear
{

class GammarayObjectSelector;

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
    void acceptConnection();
    void readScript();

private:
    // This class is owned by JS engine
    shakespear::GammarayObjectSelector* m_selector;
    std::unique_ptr<QScriptEngine> m_engine;

    QTimer m_startupTimer;

    std::unique_ptr<QTcpServer> m_server;
    std::unique_ptr<rio::logger::Log> m_logger;

    QDataStream m_inputStream;
};

} // namespace shakespear
