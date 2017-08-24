#include "Market.h"
#include "CurveDiscount.h"

ptr_disc_curve_t Market::build_discount_curve(const string& name) const
{
    return ptr_disc_curve_t(new CurveDiscount(m_mds, m_today, string("USD")));
}