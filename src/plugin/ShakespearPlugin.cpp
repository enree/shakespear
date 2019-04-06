#include "ShakespearPlugin.h"

#include "GammarayObjectSelector.h"
#include "NetworkClient.h"
#include "Paths.h"
#include "shakespear/Translate.h"

#include "core/MessageCodec.h"

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
    , m_codec(std::make_unique<MessageCodec>())
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
        m_codec.get(),
        &MessageCodec::decode);
    connect(
        m_codec.get(),
        &MessageCodec::testCase,
        this,
        &ShakespearPlugin::executeTestCase);
}

void ShakespearPlugin::initialize()
{
    auto probe = GammaRay::Probe::instance();
    if (probe->isInitialized())
    {
        m_client->connectToHost();

        m_startupTimer.stop();
        LOG_INFO << tr("Probe initialized");

        auto model = probe->objectTreeModel();
        auto selectorObject
            = new shakespear::GammarayObjectSelector(*model, *m_engine);

        auto selector = m_engine->newQObject(selectorObject);
        m_engine->globalObject().setProperty("ObjectSelector", selector);
        importModule(":/js/core.mjs");
        //        evaluate("function findObject(selector) { var object = "
        //                 "Shakespear.findObject(selector); return object;}");
    }
}

void ShakespearPlugin::executeTestCase(const TestCase& testCase)
{
    LOG_DEBUG << tr("Test case received: \n%1").arg(testCase.name);

    auto block = m_codec->encode(
        CustomMessage{ tr("Executing test case %1").arg(testCase.name) });
    m_client->write(block);

    evaluate(testCase);
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

void ShakespearPlugin::evaluate(const TestCase& testCase)
{
    TestResult testResult;
    testResult.name = testCase.name;

    auto result = m_engine->evaluate(testCase.script);
    if (result.isError())
    {
        testResult.code = TestResult::Code::INVALID_TEST_CASE;
        QString error
            = tr("Error evaluating script: %1. Line: %2")
                  .arg(result.toString(), result.property("line").toString());
        testResult.texts = QStringList() << error;
        LOG_DEBUG << error;
    }
    else
    {
        testResult.code = TestResult::Code::PASSED;
    }

    m_client->write(m_codec->encode(testResult));
}

ShakespearPlugin::~ShakespearPlugin() = default;

} // namespace shakespear
