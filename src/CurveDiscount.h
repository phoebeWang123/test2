#pragma once

#include <cmath>

#include "ICurve.h"
#include "MarketDataServer.h"

struct CurveDiscount : ICurveDiscount
{
    virtual string name() const { return m_name; }

    CurveDiscount(const Market *mkt, const Date& today, const string& ccyname)
        : m_today(today)
        , m_name("IR.DISCOUNT." + ccyname)
        , m_rate(mkt->get_yield(ccyname))
    {
    }

    // compute the discount factor
    double df(const Date& t) const
    {
        MYASSERT(!(t < m_today), "cannot get discount factor for date in the past: " << t);
        double dt = time_frac(m_today,t);
        return std::exp(-m_rate * dt);
    }

    virtual Date today() const { return m_today; }

private:
    Date   m_today;
    string m_name;
    double m_rate;
};
