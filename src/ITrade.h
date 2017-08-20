#pragma once

#include <vector>
#include <string>

#include "IObject.h"

struct ITrade : IObject
{
    // return the list of underlyings the trade depends on
    virtual std::vector<std::string> underlyings() const = 0;

    // return the quantity of the trade (the sign determines if it is a buy or sell
    virtual double quantity() const = 0;
};
