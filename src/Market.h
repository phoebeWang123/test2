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
        auto iter = m.insert(std::make_pair(name, PT()));
        if (iter.second)
            iter.first->second = (this->*fun)(name);
        return iter.first->second;
    }


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

private:
    Date m_today;
    const MarketDataServer *m_mds;
    mutable std::map<string, ptr_disc_curve_t> m_disc_curves;
};

