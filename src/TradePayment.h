#pragma once

#include "Trade.h"


struct TradePayment : Trade
{
    static const guid_t m_id;
    static const std::string m_name;

    TradePayment() {}

    void init(const std::string& underlying, double quantity, const Date& delivery_date)
    {
        Trade::init(underlying, quantity);
        m_delivery_date = delivery_date;
    }

    virtual const guid_t& id() const
    {
        return m_id;
    }

    virtual const std::string& idname() const
    {
        return m_name;
    }

    virtual ppricer_t pricer() const;

    const Date& delivery_date() const
    {
        return m_delivery_date;
    }

private:
    virtual void save_details(std::ofstream& os) const
    {
        os << m_delivery_date;
    }

    virtual void load_details(std::istringstream& is)
    {
        is >> m_delivery_date;
    }

    virtual void print_details(std::ostream& os) const
    {
        os << "Delivery Date: " << m_delivery_date << std::endl;
    }

private:
    Date m_delivery_date;
};
