#pragma once

#include "Date.h"

std::vector<short> Date::month_with_thirty_days = {2,4,6,9,11};

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

// To be implemented

//long operator-(const Date& d1, const Date& d2)
//{
//	const unsigned short total_days = 365;
//	short year_interval = d1.m_y - d2.m_y - 1;
//	unsigned short jan_to_first_date = get_YTD_days(d1);
//	unsigned short second_date_to_dec = total_days - get_YTD_days(d2);
//	short l = get_number_of_leap_years(d1, d2);
//	return (year_interval * 365 + jan_to_first_date + second_date_to_dec + l);
//}