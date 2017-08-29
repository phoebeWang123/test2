#include "MarketDataServer.h"
#include "Macros.h"
#include <limits>

const MarketDataServer *MarketDataServer::p = NULL;

MarketDataServer::MarketDataServer()
{

    /* FIXME: move this to a txt file. When reading the file with format:

        // all spot are against USD, some are in direct quotation, some in invesre quotation
        // cross spots, if needed can be constructed using the triangle rule via USD
        FX.SPOT.EUR.USD=1.12
        FX.SPOT.GBP.USD=1.52

    */


    // all spot are against USD, some are in direct quotation, i.e. price of the currency in USD
    // cross spots, if needed can be constructed using the triangle rule via USD
    m_data["FX.SPOT.EUR"] = 1.12;
    m_data["FX.SPOT.GBP"] = 1.52;
    m_data["FX.SPOT.JPY"] = 1.0/101.8;

    // interest rates (assumed flat)
    m_data["IR.USD"] = 0.03;
    m_data["IR.EUR"] = 0.06;
    m_data["IR.GBP"] = 0.04;
    m_data["IR.JPY"] = 0.01;
}

double MarketDataServer::get(const string& name) const
{
    auto iter = m_data.find(name);
    MYASSERT(iter != m_data.end(), "Market data not found: " << name);
    return iter->second;
}

std::pair<double,bool> MarketDataServer::lookup(const string& name) const
{
    auto iter = m_data.find(name);
    if(iter != m_data.end()) // found?
        return std::make_pair(iter->second, true);
    else
        return std::make_pair(std::numeric_limits<double>::quiet_NaN(), false);

}
