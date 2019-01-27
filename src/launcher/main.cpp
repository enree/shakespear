#include "appkit/AppManifest.h"
#include "appkit/Paths.h"

#include "shakespear/Paths.h"
#include "shakespear/Translate.h"

#include "coriolis/config/ConfigParser.h"
#include "coriolis/config/PTreeRW.h"

#include <QCoreApplication>

using namespace shakespear;

int main(int argc, char** argv)
{
    QCoreApplication::setOrganizationName(QStringLiteral("enree"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("github.com"));
    QCoreApplication::setApplicationName(QStringLiteral("shakespear"));

    QCoreApplication application(argc, argv);

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
}
