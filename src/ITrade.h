#pragma once

#include <vector>
#include <memory>

#include "IObject.h"
#include "Streamer.h"

// FIXME: introduce a porper serializable guid class
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
    virtual void save(std::ofstream& os) const = 0;
    virtual void load(std::istringstream& is) = 0;

    // return option type in human readable format
    virtual const std::string& idname() const = 0;

    // print trade attributes
    virtual void print(std::ostream& os) const  = 0;
};

typedef std::shared_ptr<ITrade> ptrade_t;
typedef std::vector<ptrade_t> portfolio_t;

