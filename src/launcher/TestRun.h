#pragma once

#include "gammaray/launcher/launcher.h"

#include <QObject>

namespace shakespear
{

/**
 * Test run defines single AUT run and may contains several test cases
 */
class TestRun : public QObject
{
    Q_OBJECT
public:
    explicit TestRun(
        const GammaRay::LaunchOptions& options, QObject* parent = nullptr);

    void run();
    void stop();

signals:
    void autReady();

private:
    GammaRay::Launcher m_launcher;
};

TestRun* createTestRun(
    const GammaRay::LaunchOptions& options, QObject* parent = nullptr);

} // namespace shakespear
