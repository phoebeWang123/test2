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

    virtual void save(std::ostream& os) const
    {
        // FIXME: save the entir vector object
        // FIXME: make the Date class serializable
        os << m_id << ";" << underlyings()[0] << ";" << quantity() << "\n";
    }

    virtual const std::string& idname() const
    {
        return m_name;
    }

private:
    Date m_delivery_date;
};
