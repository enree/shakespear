#include "ShakespearPlugin.h"

#include "GammarayObjectSelector.h"
#include "Paths.h"

#include "coriolis/qt/StringUtils.h"

#include <gammaray/core/probe.h>

#include <QPushButton>
#include <QScriptEngine>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QWidget>

namespace shakespear
{

ShakespearPlugin::ShakespearPlugin(GammaRay::Probe* probe, QObject* parent)
    : QObject(parent)
    , m_engine(std::make_unique<QScriptEngine>())
    , m_server(std::make_unique<QTcpServer>())
{
    // Initialize logger
    const std::string logConfig
        = rio::configPath() + "/" + "shakespear.log.ini";
    m_logger.reset(new rio::logger::Log(logConfig));

    Q_UNUSED(probe);
    constexpr auto initTimeout = 100;
    connect(
        &m_startupTimer, &QTimer::timeout, this, &ShakespearPlugin::initialize);
    m_startupTimer.start(initTimeout);

    connect(
        m_server.get(),
        &QTcpServer::newConnection,
        this,
        &ShakespearPlugin::acceptConnection);
}

void ShakespearPlugin::initialize()
{
    auto probe = GammaRay::Probe::instance();
    if (probe->isInitialized())
    {
        m_startupTimer.stop();
        SHAKESPEAR_INFO("initialized", "Probe");

        auto model = probe->objectTreeModel();
        m_selector = new shakespear::GammarayObjectSelector(*model);

        auto selector = m_engine->newQObject(m_selector);
        m_engine->globalObject().setProperty("Shakespear", selector);

        m_server->listen(QHostAddress("192.168.2.132"), 56000);

        m_engine->evaluate(
            "var lineEdit = Shakespear.findObject('.QLineEdit'); "
            "lineEdit.text = 'Welcome, JS2';"
            "Shakespear.findObject('QLabel').setText('New label');");

        try
        {
            m_selector->object<QWidget>("#Widget")->move(300, 300);
            m_selector->object<QPushButton>(".QPushButton")->setText("dsscdc");
        }
        catch (const shakespear::exception::LookupError& ex)
        {
        }
    }
}

void ShakespearPlugin::acceptConnection()
{
    SHAKESPEAR_INFO("acceptConnection", "ShakespearPlugin");
    auto socket = m_server->nextPendingConnection();

    connect(
        socket, &QAbstractSocket::disconnected, socket, &QObject::deleteLater);

    connect(
        socket,
        &QAbstractSocket::readyRead,
        this,
        &ShakespearPlugin::readScript);

    m_inputStream.setDevice(socket);
    m_inputStream.setVersion(QDataStream::Qt_5_12);
}

void ShakespearPlugin::readScript()
{
    m_inputStream.startTransaction();

    QString script;
    m_inputStream >> script;

    if (m_inputStream.commitTransaction())
    {
        SHAKESPEAR_INFO("received", "Script") << rio::strings::toUtf8(script);

        m_engine->evaluate(script);
    }
}

ShakespearPlugin::~ShakespearPlugin() = default;

} // namespace shakespear
