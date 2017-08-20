#pragma once

#include "ITrade.h"

struct Trade : ITrade
{
    virtual double quantity() const
    {
        return m_quantity;
    }

    virtual std::vector<std::string> underlyings() const
    {
        return m_underlyings;
    }

    void init(const std::string& underlying, double quantity)
    {
        m_underlyings.assign(1, underlying);
        m_quantity = quantity;
    }

    void init(const std::vector<std::string>& underlyings, double quantity)
    {
        m_underlyings = underlyings;
        m_quantity = quantity;
    }


private:
    std::vector<std::string> m_underlyings;
    double m_quantity;
};
