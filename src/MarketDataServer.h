#pragma once

#include <map>
#include <memory>
#include "Global.h"

// This is a dummy object that in a real system should be replaced by a server providing
// with real time (or historical) market data on demand and capable to produce snapshots of data.
// For the purpose of this example this is simply a singleton (see singleton pattern) object
// containing some stale pre-loaded market info.
struct MarketDataServer
{
private:
    MarketDataServer();  // note the provate constructor
    static const MarketDataServer *p;

public:
    static const MarketDataServer *instance()
    {
        // Care should be taken for the singleton construction to be be thread-safe (the code below is not!)
        if (!p)
            p = new MarketDataServer();
        return p;
    }

    double get(const string& name) const;
    std::pair<double, bool> lookup(const string& name) const;

private:
    // for simplicity, assumes market data can only have type double
    std::map<string,double> m_data;
};





