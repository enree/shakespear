#pragma once

#include "Paths.h"

#include "coriolis/app/AppManifest.h"
#include "coriolis/config/ConfigParser.h"

#include <QCoreApplication>

#include <boost/noncopyable.hpp>

namespace shakespear
{

namespace appkit
{

class Application : private boost::noncopyable
{
public:
    Application(
        int argc,
        char** argv,
        const rio::app::AppManifest& manifest,
        QCoreApplication* app,
        appkit::Paths paths);

    virtual int run();

private:
    // Add specific application options
    virtual void addSpecificOptions(rio::config::ConfigParser& configParser);

private:
    int m_argc;
    char** m_argv;
    const rio::app::AppManifest& m_manifest;
    rio::config::ConfigParser m_configParser;
    appkit::Paths m_paths;
};

} // namespace appkit

} // namespace shakespear
