#include "app/ReturnCode.h"

#include "launcher/TestRunner.h"
#include "ui/MainForm.h"

#include "ui/LogWidget.h"
#include "ui/logging/TextEditSinkHubFactory.h"
#include "ui/logging/TextEditSinkWidget.h"

#include "log/Log.h"
#include "shakespear/Paths.h"

#include <QApplication>

using namespace shakespear;
using namespace GammaRay;

namespace
{

QWidget* createLogWidgetHub()
{
    auto widget = new ui::LogWidget;

    auto hubFactory
        = boost::make_shared<logger::TextEditSinkHubFactory>(widget);
    logger::initTextEditSinkHubFactory(hubFactory);
    widget->setWindowTitle(QObject::tr("Application output"));

    return widget;
}

} // namespace

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

    // WARNING: logWidget SHOULD be created before TestRunner
    auto logWidget = createLogWidgetHub();

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

    MainForm mainForm(runner, logWidget);
    mainForm.show();

    return runner->run();
}
