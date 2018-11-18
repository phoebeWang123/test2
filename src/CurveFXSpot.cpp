#include "CurveFXSpot.h"
#include "Global.h"
#include "Market.h"

namespace minirisk {

CurveFXSpot::CurveFXSpot(Market* mkt, const Date & today, string & curve_name)
    : m_mkt(mkt)
    , m_today(today)
    , m_name(curve_name)
    {}

double CurveFXSpot::spot() const
{
    string ccyname_target = m_name.substr(fx_spot_prefix.length(), 3);
    string ccyname_base = m_name.length() == (fx_spot_prefix.length()+4) ? "USD" : m_name.substr(fx_spot_prefix.length() + 4, 3);
    double spot_target = ccyname_target == string("USD") ? 1 : m_mkt->get_fx_spot(string(fx_spot_prefix + ccyname_target + ".USD"));
    double spot_base = ccyname_base == string("USD") ? 1 : m_mkt->get_fx_spot(string(fx_spot_prefix + ccyname_base + ".USD"));
    double spot_rate = spot_target / spot_base;
    return spot_rate;
}
	
} // namespace minirisk
