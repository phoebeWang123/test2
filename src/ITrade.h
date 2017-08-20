#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "IObject.h"

typedef unsigned guid_t;

struct ITrade : IObject
{
    // return the list of underlyings the trade depends on
    virtual std::vector<std::string> underlyings() const = 0;

    // return the quantity of the trade (the sign determines if it is a buy or sell
    virtual double quantity() const = 0;

    // returns the global unique identfier assigned to this trade
    virtual const guid_t& id() const = 0;

    // serialization funcions
    virtual void save(std::ostream& os) const = 0;
};
