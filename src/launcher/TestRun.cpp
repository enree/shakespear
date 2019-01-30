#include "TestRun.h"

#include "InvalidRunOptions.h"

#include "gammaray/launcher/launchoptions.h"

namespace shakespear
{

TestRun::TestRun(const GammaRay::LaunchOptions& options, QObject* parent)
    : QObject(parent), m_launcher(options)
{
    connect(&m_launcher, &GammaRay::Launcher::started, this, &TestRun::autReady);
}

void TestRun::run()
{
    m_launcher.start();
}

void TestRun::stop() {}

TestRun* createTestRun(const GammaRay::LaunchOptions& options, QObject* parent)
{
    if (!options.isValid())
    {
        BOOST_THROW_EXCEPTION(exception::InvalidRunOptions());
    }
    return new TestRun(options, parent);
}

} // namespace shakespear
