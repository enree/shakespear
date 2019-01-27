#include "Paths.h"

#include "shakespear/Translate.h"

#include <iomanip>
#include <iostream>

namespace shakespear
{

namespace appkit
{

std::ostream& operator<<(std::ostream& out, const Paths& paths)
{
    out << SHAKESPEAR_TR("Config path: ") << paths.configPath << std::endl
        << SHAKESPEAR_TR("Plugin path: ") << paths.pluginPath << std::endl
        << SHAKESPEAR_TR("Cache path: ") << paths.cachePath << std::endl
        << SHAKESPEAR_TR("Log path: ") << paths.logPath << std::endl;
    return out;
}

} // namespace appkit

} // namespace shakespear
