#pragma once
#include "TradePayment.h"

//Buy/Sell/REC/PAY_QTY_Type_"Underlying + DeliveryDate + C/P + strike"_(other attributes)_TypeID

//Example:

//Pay_100_Payment_USD20200723_0

//Sell_10_EuropeanOption_1_EURUSD170924C1.20_1

//Buy_50_AsianOption_CNYUSD180224P6.69_Discrete_Geometric_2



const guid_t TradePayment::m_id = 0;
const std::string TradePayment::m_name = "Payment";

// My thoughts:
// Just create a map to store Type Id
// a[0]= "payment", a[1] = "European Option", a[1] = "Asian option"
// So every time we create a new asset class, we just assign an ID (static member), from which we can get the asset class type
// We then use the trade object to create an GUID, save the GUID (because it alreadly contains enough information)
// When we load the file, we are actually parsing these GUIDs.



