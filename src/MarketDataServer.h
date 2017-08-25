#pragma once

#include <map>
#include "Global.h"

// This is a dummy object that in a real system should be replaced by a server providing
// with real time (or historical) market data on demand and capable to produce snapshots of data.
// For the purpose of this example this simply serves to clients some stale pre-loaded market info.
struct MarketDataServer
{
public:
    MarketDataServer();

    double get(const string& name) const;
    std::pair<double, bool> lookup(const string& name) const;

private:
    // for simplicity, assumes market data can only have type double
    std::map<string,double> m_data;
};





