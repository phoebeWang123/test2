#pragma once

#include "IPricer.h"
#include "TradePayment.h"

struct PaymentPricer : IPricer
{
    PaymentPricer(const TradePayment& trd);

    virtual double price(const Market& m) const;

private:
    double m_amt;
    Date   m_dt;
    string m_ccy;
    string m_fx_ccy;
};

