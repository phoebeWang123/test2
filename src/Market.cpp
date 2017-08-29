#include "Market.h"
#include "CurveDiscount.h"

ptr_disc_curve_t Market::build_discount_curve(const string& name) const
{
    return ptr_disc_curve_t(new CurveDiscount(this, m_today, string("USD")));
}

const double Market::getYield(const string& ccyname) const
{
    string name("IR." + ccyname);
    auto ins = m_data_points.emplace(name, std::numeric_limits<double>::quiet_NaN());
    if (ins.second) // just inserted, need to be populated
        ins.first->second = m_mds->get(name);
    return ins.first->second;
};

