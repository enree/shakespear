#include "ShakespearPlugin.h"

#include "GammarayObjectSelector.h"
#include "NetworkClient.h"
#include "Paths.h"
#include "shakespear/Translate.h"

#include "qt/Strings.h"
#include "qt/TranslationInstaller.h"

#include <gammaray/core/probe.h>

#include <QJSEngine>
#include <QPushButton>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QWidget>

#include <boost/algorithm/string/join.hpp>

namespace shakespear
{

constexpr auto port = 56000;
constexpr auto timeout = 500;
constexpr auto attempts = 5;

ShakespearPlugin::ShakespearPlugin(GammaRay::Probe* probe, QObject* parent)
    : QObject(parent)
    , m_engine(std::make_unique<QJSEngine>())
    , m_client(std::make_unique<NetworkClient>(
          QHostAddress("127.0.0.1"), port, timeout, attempts))
{
    // Initialize logger
    const std::string logConfig
        = appkit::configPath() + "/" + "shakespear.log.ini";
    m_logger.reset(new appkit::logger::Log(logConfig));

    Q_UNUSED(probe);
    constexpr auto initTimeout = 100;
    connect(
        &m_startupTimer, &QTimer::timeout, this, &ShakespearPlugin::initialize);
    m_startupTimer.start(initTimeout);

    connect(
        m_client.get(),
        &NetworkClient::received,
        this,
        &ShakespearPlugin::processIncomingMessage);
}

void ShakespearPlugin::initialize()
{
    auto probe = GammaRay::Probe::instance();
    if (probe->isInitialized())
    {
        m_startupTimer.stop();
        LOG_INFO << tr("Probe initialized");

        auto model = probe->objectTreeModel();
        m_selector = new shakespear::GammarayObjectSelector(*model, *m_engine);

        auto selector = m_engine->newQObject(m_selector);
        m_engine->globalObject().setProperty("Shakespear", selector);
        importModule(":/js/core.mjs");
        //        evaluate("function findObject(selector) { var object = "
        //                 "Shakespear.findObject(selector); return object;}");

        m_client->connectToHost();
    }
}

void ShakespearPlugin::processIncomingMessage(QByteArray message)
{
    // Decode and evaluate
    //    LOG_INFO << tr("Script received: \n%1").arg(script);
    //    evaluate(script);
}

void ShakespearPlugin::importModule(const QString& name)
{
    QJSValue module = m_engine->importModule(name);
    QJSValue sumFunction = module.property("findObject");
    //    QJSValue result = sumFunction.call(args);

    if (sumFunction.isError())
    {
        LOG_ERROR
            << tr("Error importing module %2").arg(sumFunction.toString());
    }
    else
    {
        LOG_INFO << tr("Symbols %1 from %2 imported succesfully")
                        .arg(sumFunction.toString(), name);
    }
}

void ShakespearPlugin::evaluate(const QString& script)
{
    auto result = m_engine->evaluate(script);
    if (result.isError())
    {
        LOG_ERROR
            << tr("Error evaluating script: %1. Line: %2")
                   .arg(result.toString(), result.property("line").toString());
    }
}

ShakespearPlugin::~ShakespearPlugin() = default;

} // namespace shakespear
