#pragma once

#include "Macros.h"
#include <math.h>
#include <algorithm>
#include <string>
#include <array>


struct Date
{
private:
    static bool is_leap_year(unsigned);
    static std::string padding_dates(unsigned);
    static unsigned count_leap_years(unsigned y1, unsigned y2);

    // number of days elapsed from beginning of the year
    unsigned day_of_year() const;

    friend long operator-(const Date& d1, const Date& d2);

    static const std::array<unsigned,12> days_in_month;
    static const std::array<unsigned, 12> days_ytd;

public:
    static const unsigned short days_in_normal_year = 365;

    // Default constructor
    Date() : m_y(1970), m_m(1), m_d(1), m_is_leap(false) {}

    // Constructor where the input value is checked.
    Date(unsigned short year, unsigned short month, unsigned short day)
        : m_y(year), m_m(month), m_d(day), m_is_leap(is_leap_year(year))
    {
        check_valid();
    }

    void init(unsigned short year, unsigned short month, unsigned short day)
    {
        m_y = year;
        m_m = month;
        m_d = day;
        check_valid();
    }

    void check_valid();

    bool operator<(const Date& d) const
    {
        return (m_y < d.m_y) || (m_y == d.m_y && (m_m < d.m_m || (m_m == d.m_m && m_d < d.m_d) ) );
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

private:
    unsigned short m_y;
    unsigned short m_m;
    unsigned short m_d;
    bool m_is_leap;
};

long operator-(const Date& d1, const Date& d2);

inline double time_frac(const Date& d1, const Date& d2)
{
    return static_cast<double>(d2-d1) / Date::days_in_normal_year;
}
