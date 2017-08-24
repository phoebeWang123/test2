#pragma once

#include <memory>

#include "IObject.h"

struct IPricer : IObject
{
};


typedef std::shared_ptr<const IPricer> ppricer_t;