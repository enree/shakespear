#pragma once

#include "coriolis/exception/General.h"
#include "coriolis/qt/StringUtils.h"

#include <QString>

namespace shakespear
{

namespace exception
{

class InvalidSelector : public rio::exception::General
{
public:
    explicit InvalidSelector(const QString& selector)
    {
        (*this) << (rio::exception::ExceptionInfo(
            rio::strings::toUtf8(selector)));
    }
};

} // namespace exception

} // namespace shakespear
