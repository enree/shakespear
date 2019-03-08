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

#include "qt/Strings.h"

#include <QCoreApplication>

namespace shakespear
{

std::string Manifest::id() const
{
    return "shakespear";
}

std::string Manifest::name() const
{
    return SHAKESPEAR_TR("'Shakespear' project");
}

std::string Manifest::description() const
{
    return SHAKESPEAR_TR("Automated Qt-GUI test tool");
}

std::string Manifest::organization() const
{
    return strings::toUtf8(QCoreApplication::organizationName());
}

const appkit::AppManifest& manifest()
{
    static const Manifest appManifest;
    return appManifest;
}

} // namespace shakespear
