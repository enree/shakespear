#include "TestRunner.h"

#include "shakespear/Log.h"
#include "shakespear/Translate.h"

#include "InvalidRunOptions.h"
#include "coriolis/qt/StringUtils.h"
#include "coriolis/qt/streamSupport.h"

#include <gammaray/launcher/probeabi.h>
#include <gammaray/launcher/probefinder.h>

namespace shakespear
{

namespace
{

using namespace rio;

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
        for (const auto& availableAbi: ProbeFinder::listProbeABIs())
        {
            availableProbes
                += QString("%1(%2)")
                       .arg(availableAbi.id(), availableAbi.displayString());
        }

        SHAKESPEAR_FATAL("Config", "abi") << SHAKESPEAR_TR(
            "Invalid probe abi: {1}. Available probes are: {2}",
            abiString,
            strings::toUtf8(availableProbes.join(", ")));

        BOOST_THROW_EXCEPTION(exception::InvalidAbi(abiString));
    }
    m_options.setProbeABI(abi);
}

void TestRunnerConfig::setAut(const QString& aut)
{
    m_options.setLaunchArguments(aut.split(" "));
}

TestRunner::TestRunner(
    int argc,
    char** argv,
    const rio::app::AppManifest& manifest,
    QCoreApplication* app,
    appkit::Paths paths)
    : appkit::Application(argc, argv, manifest, app, std::move(paths))
{
}

void TestRunner::runTestSuite()
{
    try
    {
        auto testRun = new TestRun(m_config.options());
        //        QObject::connect(
        //            testRun,
        //            &TestRun::autReady,
        //            application.instance(),
        //            &QCoreApplication::quit);
        testRun->run();
    }
    catch (const shakespear::exception::InvalidRunOptions& ex)
    {
        SHAKESPEAR_ERROR("Application", "start") << ex.what();
    }
}

void TestRunner::addSpecificOptions(rio::config::ConfigParser& configParser)
{
    configParser.addSubtreeOption(
        "runner",
        SHAKESPEAR_TR("Runner options"),
        createTestRunnerConfigParser(m_config));
}

} // namespace shakespear
