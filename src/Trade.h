#pragma once

#include "ITrade.h"
#include "Streamer.h"

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
    virtual void print_details(std::ostream& os) const = 0;
    virtual void save_details(std::ofstream& os) const = 0;

protected:
    virtual void print(std::ostream& os) const
    {
        os << "Id:         " << id()                << std::endl;
        os << "Name:       " << idname()            << std::endl;
        os << "Underlying: " << m_underlyings       << std::endl;
        os << "Quantity:   " << quantity()          << std::endl;
        print_details(os);
        os << std::endl;
    }

    virtual void save(std::ofstream& os) const
    {
        os << id()                << separator
           << idname()            << separator
           << m_underlyings       << separator
           << quantity()          << separator;
        save_details(os);
        os << std::endl;
    }

private:
    std::vector<std::string> m_underlyings;
    double m_quantity;
};
