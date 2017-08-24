#include "TradePayment.h"
#include "PricerPayment.h"

ppricer_t TradePayment::pricer() const
{
    return ppricer_t(new PaymentPricer(*this));
}