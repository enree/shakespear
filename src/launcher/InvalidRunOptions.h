#pragma once

#include "coriolis/exception/General.h"
#include "coriolis/exception/KeyInfo.h"
#include "coriolis/qt/streamSupport.h"

#include <QString>

namespace shakespear
{

namespace exception
{

class InvalidRunOptions : public rio::exception::General
{
};

class InvalidAbi : public InvalidRunOptions
{
public:
    explicit InvalidAbi(const QString& abi)
    {
        (*this) << rio::exception::toKeyInfo(abi);
    }
};

} // namespace exception

} // namespace shakespear
