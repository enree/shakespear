/** @file
 * @brief Application description class
 *
 * @ingroup
 *
 * @copyright  (C) 2016 PKB RIO Design Department
 *
 * $Id: $
 */

#pragma once

#include "coriolis/app/AppManifest.h"

#include <iostream>
#include <string>

namespace shakespear
{

namespace appkit
{

/**
 * This class contains application descriptions
 * One should inherit this class to provide additional agent information
 */
class AppManifest : public rio::app::AppManifest
{
public:
    std::string id() const override;
    std::string name() const override;
    std::string description() const override;
    std::string organization() const override;
};

} // namespace appkit

} // namespace shakespear

namespace rio
{

/**
 * Return ruberoid Application manifest.
 */
const app::AppManifest& manifest();

} // namespace rio
