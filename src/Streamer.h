#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Global.h"
#include "Date.h"
#include "Macros.h"

using std::size_t;

struct my_ifstream
{
    std::istringstream m_is;
    my_ifstream(const std::string& is) : m_is(is) {}
    operator std::istream&() { return m_is; }
};

inline std::istringstream getToken(std::istream& is)
{
    std::string tmp;
    std::getline(is, tmp, separator);
    return std::istringstream(tmp);
}

template <typename T, typename A>
inline std::ostream& operator<<(std::ostream& os, const std::vector<T,A>& v)
{
    std::for_each(v.begin(), v.end(), [&os](auto i){ os << i << separator; });
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
inline my_ifstream& operator>>(my_ifstream& is, std::vector<T,A>& v)
{
    size_t sz;
    getToken(is) >> sz;
    std::cout << "sz= " << sz << "\n";
    v.resize(sz);
    for(size_t i = 0; i < sz; ++i) {
        getToken(is) >> v[i];  // read value
        std::cout << "v[i]= " << v[i] << "\n";
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

inline my_ifstream& operator>>(my_ifstream& is, Date& v)
{
    // FIXME: implement date reader
    string tmp;
    getToken(is) >> tmp;
    unsigned y = std::atoi(tmp.substr(0,4).c_str());
    unsigned m = std::atoi(tmp.substr(4,2).c_str());
    unsigned d = std::atoi(tmp.substr(6,2).c_str());
    v.init(y,m,d);
    return is;
}

template <typename T>
inline my_ifstream& operator>>(my_ifstream& is, T& d)
{
    getToken(is) >> d;
    return is;
}

