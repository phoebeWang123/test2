#pragma once

#include <iostream>

#include "TradePayment.h"

#if 0

// FIXME: define ptrade in a header file (types.h)
inline ptrade_t load_trade(std::istream& is)
{
    // |FIXME: just pseudo-code

    const strint_t name;

    ptrade_t p;

    // read name from stream is
    // ..

    if (id == TradePayment::name))
        p.reset(new TradePayment);
    else
        THROW( "Unknown trade type:" << id );

    p->load(is);

    return p;
}


#endif
