#include "MarketDataServer.h"
#include "Macros.h"
#include "Streamer.h"

#include <limits>

namespace minirisk {
MarketDataServer::MarketDataServer()
{
    const string filename("risk_factors.txt");

#if 0  // use hardcoded market data

    // all spot are against USD, some are in direct quotation, i.e. price of the currency in USD
    // cross spots, if needed can be constructed using the triangle rule via USD
    m_data["FX.SPOT.EUR"] = 1.12;
    m_data["FX.SPOT.GBP"] = 1.52;
    m_data["FX.SPOT.JPY"] = 1.0 / 101.8;

    // interest rates (assumed flat)
    m_data["IR.USD"] = 0.03;
    m_data["IR.EUR"] = 0.06;
    m_data["IR.GBP"] = 0.04;
    m_data["IR.JPY"] = 0.01;

    // save to file
    my_ofstream of(filename);
    std::for_each(m_data.begin(), m_data.end(),
        [&of](auto rf)
    {
        of << rf.first << rf.second;
        of.endl();
    });

#else // read market data from file

    my_ifstream is(filename);
    string tmp;
    while (is.read_line()) {
        string name;
        double value;
        is >> name >> value;
        //        std::cout << tmp << " " << name << " " << value << "\n";
        auto ins = m_data.emplace(name, value);
        MYASSERT(ins.second, "Duplicated risk factor: " << name);
    }

#endif
}

double MarketDataServer::get(const string& name) const
{
    auto iter = m_data.find(name);
    MYASSERT(iter != m_data.end(), "Market data not found: " << name);
    return iter->second;
}

std::pair<double, bool> MarketDataServer::lookup(const string& name) const
{
    auto iter = m_data.find(name);
    if (iter != m_data.end()) // found?
        return std::make_pair(iter->second, true);
    else
        return std::make_pair(std::numeric_limits<double>::quiet_NaN(), false);
}
}
