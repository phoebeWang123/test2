#include "Market.h"
#include "CurveDiscount.h"

#include <vector>

namespace minirisk {

double Market::from_mds(const string& objtype, const string& name)
{
    auto ins = m_risk_factors.emplace(name, std::numeric_limits<double>::quiet_NaN());
    if (ins.second) { // just inserted, need to be populated
        MYASSERT(m_mds, "Cannot fetch " << objtype << " " << name << " because the market data server has been disconnnected");
        ins.first->second = m_mds->get(name);
    }
    return ins.first->second;
}

const double Market::get_yield(const string& ccyname)
{
    string name(ir_rate_prefix + ccyname);
    return from_mds("yield curve", name);
};

const double Market::get_fx_spot(const string& name)
{
    return from_mds("fx spot", mds_spot_name(name));
}

void Market::set_risk_factors(const vec_risk_factor_t& risk_factors)
{
    clear();
    for (const auto& d : risk_factors) {
        auto i = m_risk_factors.find(d.first);
        MYASSERT((i != m_risk_factors.end()), "Risk factor not found " << d.first);
        i->second = d.second;
    }
}

Market::vec_risk_factor_t Market::get_risk_factors(const std::string& expr) const
{
    vec_risk_factor_t result;
    std::regex r(expr);
    for (const auto& d : m_risk_factors)
        if (std::regex_match(d.first, r))
            result.push_back(d);
    return result;
}

} // namespace minirisk
