#include "TestRunner.h"

#include "shakespear/Log.h"
#include "shakespear/Translate.h"

#include "InvalidRunOptions.h"
#include "coriolis/qt/streamSupport.h"

#include <gammaray/launcher/probeabi.h>
#include <gammaray/launcher/probefinder.h>

namespace shakespear
{

using namespace GammaRay;

GammaRay::LaunchOptions TestRunnerConfig::options() const
{
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
        SHAKESPEAR_DEBUG << SHAKESPEAR_TR("Available probes: ");
        for (const auto& availableAbi: ProbeFinder::listProbeABIs())
        {
            SHAKESPEAR_DEBUG << availableAbi.id() << " ("
                             << availableAbi.displayString() << ")";
        }

        BOOST_THROW_EXCEPTION(exception::InvalidAbi(abiString));
    }
    m_options.setProbeABI(abi);
}

void TestRunnerConfig::setAut(const QString& aut)
{
    m_options.setLaunchArguments(aut.split(" "));
}

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

} // namespace shakespear
