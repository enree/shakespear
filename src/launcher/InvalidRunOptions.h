#pragma once

#include "exception/General.h"
#include "exception/KeyInfo.h"
#include "qt/Strings.h"

#include <QString>

namespace exception
{

class InvalidRunOptions : public General
{
};

class InvalidAbi : public InvalidRunOptions
{
public:
    explicit InvalidAbi(const QString& abi)
    {
        (*this) << toKeyInfo(abi);
    }
};

} // namespace exception
