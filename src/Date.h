#pragma once

#include "Macros.h"

// Leap year must be a multiple of 4, and cannot be divisible by 100 without also being divisible by 400.
bool is_leap_year(const unsigned short& year)
{
    return ((year % 4 != 0) ? false : (year % 100 != 0) ? true : (year % 400 != 0) ? false : true);
}

struct Date
{
    Date() : m_y(1970), m_m(1), m_d(1) {}
    Date(unsigned short year, unsigned short month, unsigned short day)
        : m_y(year), m_m(month), m_d(day)
    {
        // FIXME: try and create a date using the C++ functions and if it fails throw an error
    }

    bool operator<(const Date& d) const
    {
        return m_y < d.m_y || (m_y == d.m_y && m_m < d.m_m) || (m_m == d.m_m && m_d < d.m_d);
    }
private:
    unsigned short m_y;
    unsigned short m_d;
    unsigned short m_m;
};

inline long operator-(const Date& d1, const Date& d2)
{
    NOT_IMPLEMENTED;
}

