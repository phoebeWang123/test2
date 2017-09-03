#pragma once

#include "Macros.h"
#include <string>
#include <array>


struct Date
{
public:
    static const unsigned days_in_normal_year = 365;
    static const unsigned first_year = 1900;
    static const unsigned last_year  = 2200;
    static const unsigned n_years  = last_year - first_year;

private:
    static std::string padding_dates(unsigned);
    static unsigned count_leap_years(unsigned y1, unsigned y2);

    // number of days elapsed from beginning of the year
    unsigned day_of_year() const;

    friend long operator-(const Date& d1, const Date& d2);

    static const std::array<unsigned,12> days_in_month;  // num of days in month M in a normal year
    static const std::array<unsigned, 12> days_ytd;      // num of days since 1-jan to 1-M in a normal year
    static const std::array<unsigned, n_years> days_epoch;   // num of days since 1-jan-1900 to 1-jan-yyyy (until 2200)

public:
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

    // number of days since 1-Jan-1900
    unsigned serial() const
    {
        return days_epoch[m_y - 1900] + day_of_year();
    }

    static bool is_leap_year(unsigned yr);

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
