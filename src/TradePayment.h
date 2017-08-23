#pragma once

#include "Trade.h"
#include "Date.h"

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


    virtual void save_details(std::ofstream& os) const
    {
        // FIXME: save the entire vector object
        // FIXME: make the Date class serializable
        os << m_delivery_date;
    }

    virtual void print_details(std::ostream& os) const
    {
        os << "Delivery Date: " << m_delivery_date << std::endl;
    }

private:
    Date m_delivery_date;
};
