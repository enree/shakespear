#pragma once

#include "exception/General.h"

namespace exception
{

class LookupError : public exception::General
{
};

class ObjectNotFound : public LookupError
{
};

class MultipleObjectsFound : public LookupError
{
};

} // namespace exception
