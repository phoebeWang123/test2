#pragma once

#include "Trade.h"
#include "Date.h"

struct TradePayment : Trade
{
    static const guid_t m_id;

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

private:
    Date m_delivery_date;
};