#include "PricerPayment.h"
#include "TradePayment.h"
#include "CurveDiscount.h"

PaymentPricer::PaymentPricer(const TradePayment& trd)
    : m_amt(trd.quantity())
    , m_dt(trd.delivery_date())
    , m_ccy(trd.underlyings()[0])
    , m_fx_ccy(m_ccy == "USD"? "":  m_ccy)
{
}

double PaymentPricer::price(const Market& mkt) const
{
    ptr_disc_curve_t disc = mkt.get_discount_curve(m_ccy);
    double df = disc->df(m_dt);

    // This PV is expressed in m_ccy. It must be converted in USD.
    if (!m_fx_ccy.empty())
        df *= mkt.get_fx_spot(m_fx_ccy);

    return m_amt * df;
}

