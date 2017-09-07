#pragma once

#include "Trade.h"

namespace minirisk {

struct TradePayment : Trade
{
    static const guid_t m_id;
    static const std::string m_name;

    TradePayment() {}

    void init(const std::string& ccy, double quantity, const Date& delivery_date)
    {
        Trade::init(quantity);
        m_ccy = ccy;
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

    const string& ccy() const
    {
        return m_ccy;
    }

    const Date& delivery_date() const
    {
        return m_delivery_date;
    }

private:
    virtual void save_details(my_ofstream& os) const
    {
        os << m_ccy << m_delivery_date;
    }

    virtual void load_details(my_ifstream& is)
    {
        is >> m_ccy >> m_delivery_date;
    }

    virtual void print_details(std::ostream& os) const
    {
        os << format_label("Currency") << m_ccy << std::endl;
        os << format_label("Delivery Date") << m_delivery_date << std::endl;
    }

private:
    string m_ccy;
    Date m_delivery_date;
};

} // namespace minirisk
