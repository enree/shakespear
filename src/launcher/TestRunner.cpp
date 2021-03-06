#include "TestRunner.h"

#include "shakespear/Translate.h"

#include "InvalidRunOptions.h"
#include "NetworkClient.h"

#include "gsl/gsl_assert"
#include "log/Log.h"

#include <gammaray/launcher/probeabi.h>
#include <gammaray/launcher/probefinder.h>

#include <QHostAddress>
#include <QTcpSocket>

namespace shakespear
{

namespace
{

ConfigParser createTestRunnerConfigParser(TestRunnerConfig& config)
{
    ConfigParser configParser;
    constexpr auto AUT_KEY = "aut";
    constexpr auto ABI_KEY = "abi";
    constexpr auto UI_KEY = "showUi";

    configParser.addOption<QString>(
        AUT_KEY,
        SHAKESPEAR_TR("Application under test"),
        [&config](const QString& aut) { config.setAut(aut); });

    configParser.addOption<QString>(
        ABI_KEY, SHAKESPEAR_TR("Probe abi"), [&config](const QString& abi) {
            config.setAbi(abi);
        });

    configParser.addOption<bool>(
        UI_KEY,
        SHAKESPEAR_TR("Show GammaRay UI on start"),
        [&config](bool uiEnabled) { config.setUiMode(uiEnabled); });

    return configParser;
}

} // namespace

using namespace GammaRay;

GammaRay::LaunchOptions TestRunnerConfig::options() const
{
    if (!m_options.isValid())
    {
        BOOST_THROW_EXCEPTION(exception::InvalidRunOptions());
    }

    return m_options;
}

void TestRunnerConfig::setUiMode(bool enabled)
{
    if (!enabled)
    {
        m_options.setUiMode(LaunchOptions::NoUi);
    }
}

void TestRunnerConfig::setAbi(const QString& abiString)
{
    const ProbeABI abi = ProbeABI::fromString(abiString);
    if (!abi.isValid() || ProbeFinder::findProbe(abi).isEmpty())
    {
        QStringList availableProbes;
        const auto& probeAbis = ProbeFinder::listProbeABIs();
        for (const auto& availableAbi: probeAbis)
        {
            availableProbes
                += QString("%1(%2)")
                       .arg(availableAbi.id(), availableAbi.displayString());
        }

        LOG_FATAL << QObject::tr(
                         "Invalid probe abi: %1. Available probes are: %2")
                         .arg(abiString, availableProbes.join(", "));

        BOOST_THROW_EXCEPTION(exception::InvalidAbi(abiString));
    }
    m_options.setProbeABI(abi);
}

void TestRunnerConfig::setAut(const QString& aut)
{
    m_options.setLaunchArguments(aut.split(" "));
}

constexpr auto port = 56000;
constexpr auto timeout = 500;
constexpr auto attempts = 5;

TestRunner::TestRunner(
    int argc,
    char** argv,
    const appkit::AppManifest& manifest,
    QCoreApplication* app,
    appkit::Paths paths)
    : appkit::Application(argc, argv, manifest, app, std::move(paths))
    , m_networkClient(std::make_unique<NetworkClient>(
          QHostAddress("127.0.0.1"), port, timeout, attempts))
{
    connect(m_networkClient.get(), &NetworkClient::connected, this, [this]() {
        emit message(tr("Connected to test server"));
    });

    connect(m_networkClient.get(), &NetworkClient::disconnected, this, [this]() {
        emit message(tr("Disconnected from test server"));
    });
}

TestRunner::~TestRunner()
{
    stopTestSuite();
}

void TestRunner::runTestSuite()
{
    if (m_testRun == nullptr)
    {
        try
        {
            m_testRun = new TestRun(m_config.options());

            connect(m_testRun, &TestRun::autReady, this, [this]() {
                emit message(
                    tr("AUT %1 ready")
                        .arg(m_config.options().launchArguments().join(" ")));
                m_networkClient->connectToHost();
            });

            connect(m_testRun, &TestRun::autStopped, this, [this]() {
                emit message(tr("AUT closed"));
            });

            connect(
                m_testRun,
                &TestRun::autStopped,
                m_testRun,
                &TestRun::deleteLater);

            connect(m_testRun, &TestRun::message, this, &TestRunner::message);

            m_testRun->run();
        }
        catch (const exception::InvalidRunOptions& ex)
        {
            LOG_ERROR << tr("Application failed to start. %1").arg(ex.what());
        }
    }
}

void TestRunner::stopTestSuite()
{
    if (m_testRun != nullptr)
    {
        m_testRun->interrupt();
        m_networkClient->disconnectFromHost();
    }
}

void TestRunner::runTestCase(const QString& testCase)
{
    m_networkClient->send(testCase);
}

void TestRunner::addSpecificOptions(ConfigParser& configParser)
{
    configParser.addSubtreeOption(
        "runner",
        SHAKESPEAR_TR("Runner options"),
        createTestRunnerConfigParser(m_config));
}

} // namespace shakespear
