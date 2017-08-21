#pragma once

#include "Date.h"

std::vector<short> Date::months_with_thirty_days = {2,4,6,9,11};

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

//std::string get_nearest_divisible(unsigned int n, unsigned int k, std::string)
//{
//
//}
//
//unsigned short get_number_of_leap_years(const Date& d1, const Date& d2)
//{
//	unsigned short a = d1.floor_of();
//	unsigned short b = d2.ceiling_of();
//	unsigned char centuryA = d1.y / 100;
//	unsigned char centuryB = d2.y / 100;
//	unsigned char floor_centuryA = centuryA.floor_of();
//	unsigned char ceiling_centuryB = centuryB .ceiling_of();
//	return (b-a)/4+1 - ((centuryA - centuryB + 1) - (floor_centuryA - ceiling_centuryB + 1))
//}


// To be implemented

//long operator-(const Date& d1, const Date& d2)
//{
//	const unsigned short total_days = 365;
//	short year_interval = d1.m_y - d2.m_y - 1;
//	unsigned short jan_to_first_date = d1.get_YTD_days();
//	unsigned short second_date_to_dec = total_days - d2.get_YTD_days();
//	short l = get_number_of_leap_years(d1, d2);
//	return (year_interval * 365 + jan_to_first_date + second_date_to_dec + l);
//}