#include "launcher/appkit/AppManifest.h"
#include "launcher/appkit/Application.h"
#include "launcher/appkit/Paths.h"
#include "launcher/appkit/ReturnCode.h"

#include "launcher/MainForm.h"
#include "launcher/TestRunner.h"

#include "shakespear/Log.h"
#include "shakespear/Paths.h"

#include <QApplication>

using namespace rio;
using namespace shakespear;
using namespace GammaRay;

int main(int argc, char** argv)
{
    QCoreApplication::setOrganizationName(
        QStringLiteral("yuri.borisoff@gmail.com"));
    QCoreApplication::setApplicationName(QStringLiteral("shakespear"));

    // Initialize logger
    boost::scoped_ptr<rio::logger::Log> logger;
    const std::string logConfig
        = rio::configPath() + "/" + rio::manifest().configBase() + ".log.ini";

    try
    {
        logger.reset(new rio::logger::Log(logConfig));
    }
    catch (const std::exception& exception)
    {
        std::cerr << "Logger initialization failed."
                  << " Config file: " << logConfig << " " << exception.what()
                  << std::endl;

        return shakespear::LOGGER_ERROR;
    }

    const appkit::Paths paths
        = { rio::configPath(),      rio::pluginPath(), rio::cachePath(),
            rio::translationPath(), rio::logPath(),    "aut.conf" };

    QApplication application(argc, argv);
    auto runner = std::make_shared<
        TestRunner>(argc, argv, rio::manifest(), &application, paths);

    MainForm mainForm(runner);
    mainForm.show();

    return runner->run();
}
