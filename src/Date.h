#pragma once

#include "Macros.h"

struct Date
{
    Date() : m_d(1), m_m(1), m_y(1970) {}
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

