#include "PricerPayment.h"
#include "TradePayment.h"
#include "CurveDiscount.h"

PaymentPricer::PaymentPricer(const TradePayment& trd)
    : m_amt(trd.quantity())
    , m_dt(trd.delivery_date())
    // FIXME: define these constant strings in Global.h
    , m_ccy(trd.underlyings()[0])
    , m_curve("IR.DISCOUNT." + m_ccy)
{
}

double PaymentPricer::price(const Market& mkt) const
{
    ptr_disc_curve_t disc = mkt.get_discount_curve(m_curve);
    double df = disc->df(m_dt);
    // FIXME: this PV is expressed in m_ccy. It must be converted in USD.
    return m_amt * df;
}

