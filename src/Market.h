#pragma once

#include "Global.h"
#include "IObject.h"
#include "ICurve.h"
#include "MarketDataServer.h"

#include <vector>

struct Market : IObject
{
private:
    ptr_disc_curve_t build_discount_curve(const string& name);

    // NOTE: this function is not thread safe
    template <typename PT>
    const PT& get_curve(const string& name, std::map<string, PT>& m, PT(Market::*fun)(const string&))
    {
        auto iter = m.emplace(name, PT());
        if (iter.second)
            iter.first->second = (this->*fun)(name);
        return iter.first->second;
    }

    double from_mds(const string& objtype, const string& name);

public:

    typedef std::pair<string, double> risk_factor_t;
    typedef std::vector<std::pair<string, double>> vec_risk_factor_t;

    Market(const MarketDataServer *mds, const Date& today)
        : m_today(today)
        , m_mds(mds)
    {
    }

    virtual Date today() const { return m_today; }

    const ptr_disc_curve_t& get_discount_curve(const string& name)
    {
        return get_curve(name, m_disc_curves, &Market::build_discount_curve);
    }

    // yeild rate for currency name
    const double get_yield(const string& name);

    // fx exchange rate to convert 1 unit of ccy1 into USD
    const double get_fx_spot(const string& ccy);

    // after the market has been disconnected, it is no more possible to fetch
    // new data points from the market data server
    void disconnect()
    {
        m_mds = 0;
    }

    // returns risk factor with a specified prefix and their value
    // (if the prefix is "" all risk factor are returned)
    vec_risk_factor_t get_risk_factors(const std::string& prefix) const;

    // clear all market curves execpt for the data points
    void clear()
    {
        m_disc_curves.clear();
    }

    // destroy all existing objects and modify a selected number of data points
    void set_data_points(const vec_risk_factor_t& data_points);

private:
    Date m_today;
    const MarketDataServer *m_mds;

    // market curves
    std::map<string, ptr_disc_curve_t> m_disc_curves;

    // raw risk factors
    std::map<string, double> m_data_points;
};

