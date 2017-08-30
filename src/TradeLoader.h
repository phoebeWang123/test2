#pragma once

#include "TradePayment.h"

inline ptrade_t load_trade(const string& s)
{
    std::istringstream is(s);

    string name;
    ptrade_t p;

    // read trade identifier
    guid_t id;
    getToken(is) >> id;

    if (id == TradePayment::m_id)
        p.reset(new TradePayment);
    else
        THROW( "Unknown trade type:" << id );

    p->load(is);

    return p;
}

inline void save_portfolio(const string& filename, const std::vector<ptrade_t>& portfolio)
{
    // test saving to file
    std::ofstream of(filename);
    std::for_each(portfolio.begin(), portfolio.end(), [&of](const ptrade_t &pt){ pt->save(of); });
    of.close();
}

inline std::vector<ptrade_t>  load_portfolio(const string& filename)
{
    std::vector<ptrade_t> portfolio;

    // test reloading the portfolio
    std::ifstream is(filename);
    string tmp;
    while(true) {
        getline(is, tmp);
        if (is.eof())
            break;
        portfolio.push_back(load_trade(tmp));
    }

    return portfolio;
}
