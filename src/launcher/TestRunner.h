#pragma once

#include "AppManifest.h"
#include "TestRun.h"

#include "app/Application.h"
#include "config/ConfigParser.h"

#include "gammaray/launcher/launchoptions.h"

#include <QPointer>

namespace shakespear
{

using appkit::config::ConfigParser;

class NetworkClient;

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
        const appkit::AppManifest& manifest,
        QCoreApplication* app,
        appkit::Paths paths);

    ~TestRunner() override;

signals:
    void message(QString text);

public slots:
    void runTestSuite();
    void stopTestSuite();
    void runTestCase(const QString& testCase);

private:
    void addSpecificOptions(ConfigParser& configParser) override;

private:
    TestRunnerConfig m_config;
    QPointer<TestRun> m_testRun;
    std::unique_ptr<NetworkClient> m_networkClient;
};

} // namespace shakespear
