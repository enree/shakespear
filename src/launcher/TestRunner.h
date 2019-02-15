#pragma once

#include "TestRun.h"

#include "appkit/Application.h"

#include "coriolis/config/ConfigParser.h"

#include "gammaray/launcher/launchoptions.h"

#include <QPointer>

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

class TestRunner : public QObject, public appkit::Application
{
    Q_OBJECT
public:
    TestRunner(
        int argc,
        char** argv,
        const rio::app::AppManifest& manifest,
        QCoreApplication* app,
        appkit::Paths paths);

    ~TestRunner() override;

public slots:
    void runTestSuite();
    void stopTestSuite();

private:
    void addSpecificOptions(rio::config::ConfigParser& configParser) override;

private:
    TestRunnerConfig m_config;
    QPointer<TestRun> m_testRun;
};

} // namespace shakespear
