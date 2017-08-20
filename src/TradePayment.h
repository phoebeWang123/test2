#pragma once

#include "Trade.h"
#include "Date.h"

struct TradePayment : Trade
{
    TradePayment() {}

    void init(const std::string& underlying, double quantity, const Date& delivery_date)
    {
        Trade::init(underlying, quantity);
        m_delivery_date = delivery_date;
    }

private:
    Date m_delivery_date;
};
