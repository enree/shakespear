#pragma once

#include "exception/General.h"
#include "qt/Strings.h"

#include <QString>

namespace exception
{

/**
 * Type for passing extra details to exception. One can use it to pass
 * arbitrary string to exception
 */
using Selector = boost::error_info<struct selector_, QString>;

class InvalidSelector : public exception::General
{
public:
    explicit InvalidSelector(const QString& selector)
    {
        (*this) << (Selector(selector));
    }
};

} // namespace exception
