#include "PricerPayment.h"
#include "TradePayment.h"
#include "CurveDiscount.h"

namespace minirisk {
PaymentPricer::PaymentPricer(const TradePayment& trd)
    : m_amt(trd.quantity())
    , m_dt(trd.delivery_date())
    , m_ir_curve(ir_curve_discount_prefix + trd.ccy())
    , m_fx_ccy(trd.ccy() == "USD" ? "" : trd.ccy())
{
}

double PaymentPricer::price(Market& mkt) const
{
    ptr_disc_curve_t disc = mkt.get_discount_curve(m_ir_curve);
    double df = disc->df(m_dt); // this throws an exception if m_dt<today

    // This PV is expressed in m_ccy. It must be converted in USD.
    if (!m_fx_ccy.empty())
        df *= mkt.get_fx_spot(m_fx_ccy);

    return m_amt * df;
}
}

