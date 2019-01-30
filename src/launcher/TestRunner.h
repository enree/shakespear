#pragma once

#include "coriolis/config/ConfigParser.h"

#include "gammaray/launcher/launchoptions.h"

namespace shakespear
{

using rio::config::ConfigParser;

/**
 * Test runner configuration
 */
class TestRunnerConfig
{
public:
    GammaRay::LaunchOptions options() const;

    /**
     * Set ui mode (true/false)
     */
    void setUiMode(bool enabled);
    /**
     * Set probe abi. Throws if abi not found
     */
    void setAbi(const QString& abiString);
    /**
     * Set application under test;
     */
    void setAut(const QString& aut);

private:
    GammaRay::LaunchOptions m_options;
};

ConfigParser createTestRunnerConfigParser(TestRunnerConfig& config);

class TestRunner
{
public:
};

} // namespace shakespear
