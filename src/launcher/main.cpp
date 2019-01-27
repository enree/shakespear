#include "appkit/AppManifest.h"
#include "appkit/Paths.h"
#include "appkit/ReturnCode.h"
#include "appkit/UnixSignalHandler.h"

#include "shakespear/Log.h"
#include "shakespear/Paths.h"
#include "shakespear/Translate.h"

#include "coriolis/config/ConfigParser.h"
#include "coriolis/config/PTreeRW.h"
#include "coriolis/qt/StringUtils.h"
#include "coriolis/qt/TranslationInstaller.h"

#include <QCoreApplication>

#include <boost/algorithm/string/join.hpp>

using namespace rio;
using namespace shakespear;

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

    QCoreApplication application(argc, argv);
    shakespear::appkit::installSignalHandler(&application);

    // Boost translations
    const std::vector<std::string> domains
        = translate::installBoostTranslation(translationPath());
    SHAKESPEAR_DEBUG << "Installed boost translations: "
                            + boost::algorithm::join(domains, ", ");

    // Qt translations
    const QStringList translations
        = QStringList() << rio::qt::installUserTranslations(
                               strings::fromUtf8(translationPath()))
                        << rio::qt::installSystemTranslation("qt")
                        << rio::qt::installSystemTranslation("qt_help");
    SHAKESPEAR_DEBUG << "Installed qt translations: "
                            + strings::toUtf8(translations.join(", "));

    using rio::config::ConfigParser;
    ConfigParser parser;
    ConfigParser::ProgramOptions infoOptions = parser.infoOptions(argc, argv);

    // First of all check if we need to show help
    if (infoOptions.count(ConfigParser::HELP) > 0)
    {
        parser.printUsage();
        return 0;
    }

    const appkit::Paths paths = {
        rio::configPath(), rio::pluginPath(), rio::cachePath(), rio::logPath()
    };

    // Second of all check if we need to show version
    if (infoOptions.count(ConfigParser::VERSION) > 0)
    {
        std::cout << rio::manifest() << std::endl << paths;
        return 0;
    }

    // Add tree from command line, it will overwrite config values
    parser.addPtree(
        rio::config::fromCommandLine(argc, argv, parser.keys(), false),
        SHAKESPEAR_TR("command line"));

    parser.readAll();

    QCoreApplication::exec();
}
