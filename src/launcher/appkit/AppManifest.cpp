/** @file
 * @brief
 *
 * @ingroup
 *
 * @copyright  (C) 2016 PKB RIO Design Department
 *
 * $Id: $
 */

#include "AppManifest.h"

#include "shakespear/Translate.h"

#include "coriolis/qt/StringUtils.h"

#include <QCoreApplication>

namespace shakespear
{

namespace appkit
{

using namespace rio;

std::string AppManifest::id() const
{
    return strings::toUtf8(qAppName());
}

std::string AppManifest::name() const
{
    return SHAKESPEAR_TR("'Shakespear' project");
}

std::string AppManifest::description() const
{
    return SHAKESPEAR_TR("Automated Qt-GUI test tool");
}

std::string AppManifest::organization() const
{
    return "yuri.borisoff@gmail.com";
}

} // namespace appkit

} // namespace shakespear

namespace rio
{

const app::AppManifest& manifest()
{
    static const shakespear::appkit::AppManifest appManifest;
    return appManifest;
}

} // namespace rio
