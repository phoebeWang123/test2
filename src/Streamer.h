#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "Global.h"
#include "Date.h"
#include "Macros.h"

using std::size_t;

template <typename T, typename A>
inline std::ostream& operator<<(std::ostream& os, const std::vector<T,A>& v)
{
    std::for_each(v.begin(), v.end(), [&os](auto i){ os << i << ","; });
    return os;
}

template <typename T, typename A>
inline std::ofstream& operator<<(std::ofstream& os, const std::vector<T,A>& v)
{
    os << v.size() << separator;
    std::for_each(v.begin(), v.end(), [&os](auto i){ os << i << separator; });
    return os;
}


template <typename T, typename A>
inline std::istream& operator>>(std::ifstream& is, std::vector<T,A>& v)
{
    size_t sz;
    char c;
    is >> sz;
    v.resize(sz);
    is >> c;  // read separator
    for(size_t i = 0; i < sz; ++i) {
        is >> v[i];  // read value
        is >> c; // read separator
    }
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const Date& d)
{
    os << d.to_string(true);
    return os;
}

inline std::ofstream& operator<<(std::ofstream& os, const Date& d)
{
    os << d.to_string(false);
    return os;
}


inline std::istream& operator>>(std::ifstream& is, Date& v)
{
    // FIXME: implement date reader
    NOT_IMPLEMENTED;
    return is;
}
