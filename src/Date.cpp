#include <iomanip>

#include "Date.h"

const std::array<unsigned, 12> Date::days_in_month = {31,28,31,30,31,30,31,31,30,31,30,31};
const std::array<unsigned, 12> Date::days_ytd = {0,31,59,90,120,151,181,212,243,273,304,334};

/* The function checks if a given year is a leap year.
   Leap year must be a multiple of 4, but it cannot be a multiple of 100 without also being a multiple of 400.
*/
bool Date::is_leap_year(unsigned year)
{
    return ((year % 4 != 0) ? false : (year % 100 != 0) ? true : (year % 400 != 0) ? false : true);
}

// The function pads a zero before the month or day if it has only one digit.
std::string Date::padding_dates(unsigned month_or_day)
{
    std::ostringstream os;
    os << std::setw(2) << std::setfill('0') << month_or_day;
    return os.str();
}

void Date::check_valid()
{
    MYASSERT(m_y >= 1970, "The year must be no earlier than year 1970, got " << m_y);
    MYASSERT(m_m >= 1 && m_m <= 12, "The month must be a integer between 1 and 12, got " << m_m);
    unsigned dmax = days_in_month[m_m - 1] + ((m_m == 2 && m_is_leap) ? 1 : 0);
    MYASSERT(m_d >= 1 && m_d <= 31, "The day must be a integer between 1 and 31, got " << m_d);
}

unsigned Date::doy() const
{
    return days_ytd[m_m - 1] + ((m_m > 2 && m_is_leap) ? 1 : 0) + (m_d - 1);
}


/* The function counts the number of leap years between two given years.
   Not inclusive in both ends, because the fractional part will be taken care by the member function count_YTD_days().
   We count multiple of 4 first, then count years which are multiples of 100 but not 400.
   Not implemented for the situation where y1 > y2.
*/
unsigned Date::count_leap_years(unsigned y1, unsigned y2)
{
    MYASSERT(y1 <= y2, "First year must be no less than the second year");
    double d_y1 = static_cast<double> (y1);
    short div_by_4 = y2 / 4 - short(ceil(d_y1 / 4)) + 1;
    short div_by_100 = y2 / 100 - short(ceil(d_y1 / 100)) + 1;
    short div_by_400 = y2 / 400 - short(ceil(d_y1 / 400)) + 1;
    return (y1 == y2)? 0: div_by_4 - (div_by_100 - div_by_400);
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
    int year_interval = static_cast<int>(d1.m_y - d2.m_y) - 1;
    unsigned day_of_year_1 = d1.doy();
    unsigned day_of_year_2 = d2.doy();
    if (d1.m_y == d2.m_y)
        return day_of_year_1 - day_of_year_2;
    unsigned day_to_year_end2 = 365 + ((d2.m_m > 2 && d2.m_is_leap) ? 1 : 0) - day_of_year_2;
    return day_to_year_end2 + day_of_year_1 + (d1.m_y - d2.m_y - 1) * 365 + Date::count_leap_years(d2.m_y, d1.m_y);
}

