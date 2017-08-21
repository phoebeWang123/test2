#pragma once

#include "Trade.h"

// Leap year must be a multiple of 4, but it cannot be a multiple of 100 without also being a multiple of 400.
bool is_leap_year(const unsigned short& year)
{
    return ((year % 4 != 0) ? false : (year % 100 != 0) ? true : (year % 400 != 0) ? false : true);
}

// If the month or day is less than 10, then we pad a zero before it
std::string padding_dates(const unsigned short& month_or_day)
{
    return ((month_or_day < 10) ? "0" : "" )+ std::to_string(month_or_day);
}