#pragma once

#include "Macros.h"
#include <math.h>
#include <algorithm>
#include <string>
#include <vector>

bool is_leap_year(const unsigned short& );
std::string padding_dates(const unsigned short&);
unsigned short count_leap_years(const unsigned short& y1, const unsigned short& y2, const char& number_of_leap_years_at_ends);


struct Date
{
    friend long operator-(const Date& d1, const Date& d2);

    static const std::vector<short> months_with_thirty_days;
    static const unsigned short days_in_normal_year;

    // Default constructor
    Date() : m_y(1970), m_m(1), m_d(1) {}

    // Constructor where the input value is checked.
    Date(unsigned short year, unsigned short month, unsigned short day)
        : m_y(year), m_m(month), m_d(day), is_leap(is_leap_year(year))
    {
        MYASSERT(year >= 1970, "The year must be no earlier than year 1970");
        MYASSERT(month >= 1 && month <= 12, "The month must be a integer between 1 and 12");
        MYASSERT(day >= 1 && day <= 31, "The day must be a integer between 1 and 31");
        switch (month)
        {
            case 4: case 6: case 9: case 11:
                MYASSERT(day != 31, "This month only has 30 days");
                break;
            case 2:
                MYASSERT(is_leap || (!is_leap && day != 29), "Non-leap-year February only has 28 days.");
                break;
        }
    }

    bool operator<(const Date& d) const
    {
        return (m_y < d.m_y) || (m_y == d.m_y && m_m < d.m_m) || (m_m == d.m_m && m_d < d.m_d);
    }

    bool operator==(const Date& d) const
    {
        return (m_y == d.m_y) && (m_m == d.m_m) && (m_d == d.m_d);
    }

    bool operator>(const Date& d) const
    {
        return d < (*this);
    }

    // In YYYYMMDD format
    std::string to_string(bool pretty = true) const
    {
        return pretty
                ? std::to_string(m_d) + "-" + std::to_string(m_m) + "-" + std::to_string(m_y)
                : std::to_string(m_y) + padding_dates(m_m) + padding_dates(m_d);
    }

    /* The function calculates the YTD days (first element) and the count of rest of days in the year (second element)

       We assume that the interval is left inclusive and right exclusive.
    */
    std::pair<unsigned short, unsigned short> count_YTD_days() const
    {
        // The lower bound will calculate how many 30-day months are there before the current month
        auto low = std::lower_bound(months_with_thirty_days.cbegin(), months_with_thirty_days.cend(), m_m);
        auto number_of_months_with_thirty_days = low - months_with_thirty_days.cbegin();
        // We need to minus 1 when calculating the number of 31-day months to exclude the current month
        unsigned short total = number_of_months_with_thirty_days * 30 + (m_m - number_of_months_with_thirty_days - 1) * 31 + m_d - 1;
        // If current month is later than Feb, then we need to minus 1 or 2
        unsigned short YTD = (m_m <= 2) ? total : (is_leap) ? (total - 1) : (total - 2);
        return (is_leap) ? std::make_pair(YTD, Date::days_in_normal_year + 1 - YTD) : std::make_pair(YTD, Date::days_in_normal_year - YTD);
    }

private:
    unsigned short m_y;
    unsigned short m_m;
    unsigned short m_d;
    bool is_leap;
};

long operator-(const Date& d1, const Date& d2);
