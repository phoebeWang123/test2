#pragma once
#include "ICurve.h"
#include <map>
#include <vector>

namespace minirisk {

struct Market;

struct CurveDiscount : ICurveDiscount
{
    virtual string name() const { return m_name; }

    CurveDiscount(Market *mkt, const Date& today, const string& curve_name);

    // compute the discount factor
    double df(const Date& t) const;

    virtual Date today() const { return m_today; }

private:
    Date   m_today;
    string m_name;
    double m_rate;
	std::vector<std::tuple<string, double, double, double, double>> m_rateMap;
	string test;
};

} // namespace minirisk
