#include "PricerPayment.h"
#include "TradePayment.h"
#include "CurveDiscount.h"

PaymentPricer::PaymentPricer(const TradePayment& trd)
    : m_amt(trd.quantity())
    , m_dt(trd.delivery_date())
    , m_curve("IR.DISOCUNT." + trd.underlyings()[0])
{
}

double PaymentPricer::price(const Market& mkt) const
{
    ptr_disc_curve_t disc = mkt.get_discount_curve(m_curve);
    double df = disc->df(m_dt);
    return m_amt * df;
}

