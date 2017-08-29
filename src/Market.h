#pragma once

#include "Global.h"
#include "IObject.h"
#include "ICurve.h"
#include "MarketDataServer.h"

struct Market : IObject
{
private:
    ptr_disc_curve_t build_discount_curve(const string& name) const;

    // NOTE: this function is not thread safe
    template <typename PT>
    const PT& get_curve(const string& name, std::map<string, PT>& m, PT(Market::*fun)(const string&) const) const
    {
        auto iter = m.emplace(name, PT());
        if (iter.second)
            iter.first->second = (this->*fun)(name);
        return iter.first->second;
    }

    double from_mds(const string& objtype, const string& name) const;

public:

    Market(const MarketDataServer *mds, const Date& today)
        : m_today(today)
        , m_mds(mds)
    {
    }

    virtual Date today() const { return m_today; }

    const ptr_disc_curve_t& get_discount_curve(const string& name) const
    {
        return get_curve(name, m_disc_curves, &Market::build_discount_curve);
    }

    // yeild rate for currency name
    const double get_yield(const string& name) const;

    // fx exchange rate to convert 1 unit of ccy1 into USD
    const double get_fx_spot(const string& ccy) const;

    const std::map<string, double>& data_points() const
    {
        return m_data_points;
    }

    // after the market has been disconnected, it is no more possible to fetch
    // new data points from the market data server
    void disconnect()
    {
        m_mds = 0;
    }

    // clear all market curves execpt for the data points
    void clear()
    {
        m_disc_curves.clear();
    }

private:
    Date m_today;
    const MarketDataServer *m_mds;

    // market curves
    mutable std::map<string, ptr_disc_curve_t> m_disc_curves;

    // raw risk factors
    mutable std::map<string, double> m_data_points;
};

