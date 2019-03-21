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

#include "app/AppManifest.h"

#include <iostream>
#include <string>

namespace shakespear
{

/**
 * This class contains application descriptions
 * One should inherit this class to provide additional agent information
 */
class Manifest : public appkit::AppManifest
{
public:
    std::string id() const override;
    std::string name() const override;
    std::string description() const override;
    std::string organization() const override;
};

/**
 * Return Application manifest.
 */
const appkit::AppManifest& manifest();

} // namespace shakespear
