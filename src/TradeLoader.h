#pragma once

#include "TradePayment.h"

inline ptrade_t load_trade(my_ifstream& is)
{
    string name;
    ptrade_t p;

    // read trade identifier
    guid_t id;
    is >> id;

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
    my_ofstream of(filename);
    std::for_each(portfolio.begin(), portfolio.end(), [&of](const ptrade_t &pt)
        {
            pt->save(of);
            of.endl();
        });
    of.close();
}

inline std::vector<ptrade_t>  load_portfolio(const string& filename)
{
    std::vector<ptrade_t> portfolio;

    // test reloading the portfolio
    my_ifstream is(filename);
    while(is.read_line())
        portfolio.push_back(load_trade(is));

    return portfolio;
}
