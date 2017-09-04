#pragma once

#include "IPricer.h"
#include "TradePayment.h"

namespace minirisk {
struct PaymentPricer : IPricer
{
    PaymentPricer(const TradePayment& trd);

    virtual double price(Market& m) const;

private:
    double m_amt;
    Date   m_dt;
    string m_ir_curve;
    string m_fx_ccy;
};
}

