#pragma once

#include "coriolis/exception/General.h"

namespace shakespear
{

namespace exception
{

class LookupError : public rio::exception::General
{
};

class ObjectNotFound : public LookupError
{
};

class MultipleObjectsFound : public LookupError
{
};

} // namespace exception

} // namespace shakespear
