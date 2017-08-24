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


