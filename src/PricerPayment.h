#pragma once

#include "IPricer.h"
#include "TradePayment.h"

struct PaymentPricer : IPricer
{
    PaymentPricer(const TradePayment& trd);
};

