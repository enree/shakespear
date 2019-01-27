#include "Paths.h"

#include <iomanip>
#include <iostream>

namespace shakespear
{

namespace appkit
{

std::ostream& operator<<(std::ostream& out, const Paths& paths)
{
    out << std::setw(15) << "Config path: " << paths.configPath << std::endl
        << std::setw(15) << "Plugin path: " << paths.pluginPath << std::endl
        << std::setw(15) << "Cache path: " << paths.cachePath << std::endl
        << std::setw(15) << "Log path: " << paths.logPath << std::endl;
    return out;
}

} // namespace appkit

} // namespace shakespear
