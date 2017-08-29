#include "Market.h"
#include "CurveDiscount.h"

ptr_disc_curve_t Market::build_discount_curve(const string& name) const
{
    string ccyname(name.substr(name.length()-3, 3));
    return ptr_disc_curve_t(new CurveDiscount(this, m_today, name.substr()));
}

double Market::from_mds(const string& objtype, const string& name) const
{
    auto ins = m_data_points.emplace(name, std::numeric_limits<double>::quiet_NaN());
    if (ins.second) { // just inserted, need to be populated
        MYASSERT(m_mds, "Cannot fetch " << objtype << " " << name << " because the market data server has been disconnnected");
        ins.first->second = m_mds->get(name);
    }
    return ins.first->second;
}

const double Market::get_yield(const string& ccyname) const
{
    string name("IR." + ccyname);
    return from_mds("yield curve", name);
};

const double Market::get_fx_spot(const string& ccy) const
{
    string name("FX.SPOT." + ccy + ".USD");
    return from_mds("fx spot", name);
}

