#pragma once

#include "Macros.h"

bool is_leap_year(const unsigned short& );
std::string padding_dates(const unsigned short&);

struct Date
{
    Date() : m_y(1970), m_m(1), m_d(1) {}

    Date(unsigned short year, unsigned short month, unsigned short day)
        : m_y(year), m_m(month), m_d(day)
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
                bool leap = is_leap_year(year);
                MYASSERT(leap || (!leap && day != 29), "Non-leap-year February only has 28 days.");
                break;
        }
    }

    bool operator<(const Date& d) const
    {
        return m_y < d.m_y || (m_y == d.m_y && m_m < d.m_m) || (m_m == d.m_m && m_d < d.m_d);
    }

    bool operator>(const Date& d) const
    {
        return m_y > d.m_y || (m_y == d.m_y && m_m > d.m_m) || (m_m == d.m_m && m_d > d.m_d);
    }

    std::string serialize_date() const
    {
        return std::to_string(m_y) + padding_dates(m_m) + padding_dates(m_d);
    }

private:
    unsigned short m_y;
    unsigned short m_m;
    unsigned short m_d;
};

inline long operator-(const Date& d1, const Date& d2)
{
    NOT_IMPLEMENTED;
}

