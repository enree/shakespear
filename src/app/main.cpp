#include "app/ReturnCode.h"

#include "launcher/MainForm.h"
#include "launcher/TestRunner.h"

#include "log/Log.h"
#include "shakespear/Paths.h"

#include <QApplication>

using namespace shakespear;
using namespace GammaRay;

int main(int argc, char** argv)
{
    QCoreApplication::setOrganizationName(
        QStringLiteral("yuri.borisoff@gmail.com"));
    QCoreApplication::setApplicationName(QStringLiteral("shakespear"));

    const appkit::Paths paths
        = { appkit::configPath(), appkit::pluginPath(),
            appkit::cachePath(),  appkit::translationPath(),
            appkit::logPath(),    "aut.conf" };

    QApplication application(argc, argv);
    std::shared_ptr<TestRunner> runner;

    try
    {
        runner = std::make_shared<
            TestRunner>(argc, argv, manifest(), &application, paths);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return appkit::INITIALIZATION_ERROR;
    }

    MainForm mainForm(runner);
    mainForm.show();

    return runner->run();
}
