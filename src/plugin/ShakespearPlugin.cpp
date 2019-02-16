#include "ShakespearPlugin.h"

#include "GammarayObjectSelector.h"
#include "Paths.h"
#include "shakespear/Translate.h"

#include "coriolis/qt/StringUtils.h"

#include <gammaray/core/probe.h>

#include <QJSEngine>
#include <QPushButton>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QWidget>

namespace shakespear
{

ShakespearPlugin::ShakespearPlugin(GammaRay::Probe* probe, QObject* parent)
    : QObject(parent)
    , m_engine(std::make_unique<QJSEngine>())
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
        m_selector = new shakespear::GammarayObjectSelector(*model, *m_engine);

        auto selector = m_engine->newQObject(m_selector);
        m_engine->globalObject().setProperty("Shakespear", selector);
        //        importModule(":/js/core");
        evaluate("function findObject(selector) { var object = "
                 "Shakespear.findObject(selector); return object;}");

        m_server->listen(QHostAddress("192.168.3.26"), 56000);

        evaluate("var lineEdit = findObject('.QLineEdit'); "
                 "lineEdit.text = 'Welcome, JS2';"
                 "Shakespear.findObject('QLabel').setText('New label');");
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
        evaluate(script);
    }
}

void ShakespearPlugin::importModule(const QString& name)
{
    QJSValue result = m_engine->importModule(name);
    if (result.isError())
    {
        SHAKESPEAR_ERROR("importModule", "Engine")
            << rio::strings::toUtf8(result.toString());
    }
    else
    {
        SHAKESPEAR_INFO("importModule", "Engine") << SHAKESPEAR_TR(
            "Symbols {1} from {2} imported succesfully",
            rio::strings::toUtf8(result.toString()),
            rio::strings::toUtf8(name));
    }
}

void ShakespearPlugin::evaluate(const QString& script)
{
    auto result = m_engine->evaluate(script);
    if (result.isError())
    {
        SHAKESPEAR_ERROR("evaluate", "Engine") << SHAKESPEAR_TR(
            "{1}. Line: {2}",
            rio::strings::toUtf8(result.toString()),
            rio::strings::toUtf8(result.property("line").toString()));
    }
}

ShakespearPlugin::~ShakespearPlugin() = default;

} // namespace shakespear
