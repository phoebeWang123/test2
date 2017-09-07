#pragma once

#include <cmath>

#include "ICurve.h"
#include "MarketDataServer.h"
#include "Streamer.h"

namespace minirisk {

struct CurveDiscount : ICurveDiscount
{
    virtual string name() const { return m_name; }

    CurveDiscount(Market *mkt, const Date& today, const string& curve_name)
        : m_today(today)
        , m_name(curve_name)
        , m_rate(mkt->get_yield(curve_name.substr(ir_curve_discount_prefix.length(),3)))
    {
    }

    // compute the discount factor
    double df(const Date& t) const
    {
        MYASSERT((!(t < m_today)), "cannot get discount factor for date in the past: " << t);
        double dt = time_frac(m_today, t);
        return std::exp(-m_rate * dt);
    }

    virtual Date today() const { return m_today; }

private:
    Date   m_today;
    string m_name;
    double m_rate;
};

} // namespace minirisk
