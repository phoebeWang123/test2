#include "Date.h"

const std::vector<short> Date::months_with_thirty_days = {2,4,6,9,11};
const unsigned short Date::days_in_normal_year = 365;

/* The function checks if a given year is a leap year.
   Leap year must be a multiple of 4, but it cannot be a multiple of 100 without also being a multiple of 400.
*/
bool is_leap_year(const unsigned short& year)
{
    return ((year % 4 != 0) ? false : (year % 100 != 0) ? true : (year % 400 != 0) ? false : true);
}

// The function pads a zero before the month or day if it has only one digit.
std::string padding_dates(const unsigned short& month_or_day)
{
    return ((month_or_day < 10) ? "0" : "" )+ std::to_string(month_or_day);
}

/* The function counts the number of leap years between two given years.
   Not inclusive in both ends, because the fractional part will be taken care by the member function count_YTD_days().
   We count multiple of 4 first, then count years which are multiples of 100 but not 400.
   Not implemented for the situation where y1 > y2.
*/
unsigned short count_leap_years(const unsigned short& y1, const unsigned short& y2, const char& number_of_leap_years_at_ends)
{
    MYASSERT(y1 <= y2, "First year must be no less than the second year");
    double d_y1 = static_cast<double> (y1);
    short div_by_4 = y2 / 4 - short(ceil(d_y1 / 4)) + 1;
    short div_by_100 = y2 / 100 - short(ceil(d_y1 / 100)) + 1;
    short div_by_400 = y2 / 400 - short(ceil(d_y1 / 400)) + 1;
    return (y1 == y2)? 0: div_by_4 - (div_by_100 - div_by_400) - number_of_leap_years_at_ends;
}

/*  The function calculates the distance between two Dates.
    d1 > d2 is allowed, which returns the negative of d2-d1. We will illustrate by considering the case d1 < d2 below.

    The distance of two dates consist of four parts:
    1. Complete years: all treated as 365 days.
    2. YTD days to d1
    3. The rest of days in d2.m_y (i.e., 365 or 366 - YTD days to d1)
    4. Leap years: both ends are not included (they are taken care in count_YTD_days()). Every leap year will add 1 towards the total count

    We assume that the interval is left inclusive and right exclusive.
*/
long operator-(const Date& d1, const Date& d2)
{
    if (d1 == d2) return 0;
    if (d2 > d1) return -(d2 - d1);
    unsigned short year_interval = d1.m_y - d2.m_y - 1;
    unsigned short jan_to_first_date = d1.count_YTD_days().first;
    unsigned short second_date_to_dec = d2.count_YTD_days().second;
    unsigned short l = count_leap_years(d2.m_y, d1.m_y, d1.is_leap + d2.is_leap);
    return (year_interval * Date::days_in_normal_year + jan_to_first_date + second_date_to_dec + l);
}
