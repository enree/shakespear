#pragma once

#include <string>

namespace shakespear
{

namespace appkit
{

/**
 * Main application paths
 */
struct Paths
{
    std::string configPath;
    std::string pluginPath;
    std::string cachePath;
    std::string logPath;
};

std::ostream& operator<<(std::ostream& out, const Paths& paths);

} // namespace appkit

} // namespace shakespear
