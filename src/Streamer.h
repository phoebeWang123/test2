#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Global.h"
#include "Date.h"
#include "Macros.h"

// streaming separator
const char separator = ';';

//
// Auxiliary functions
//

inline std::istringstream getToken(std::istream& is)
{
    std::string tmp;
    std::getline(is, tmp, separator);
    return std::istringstream(tmp);
}


//
// Generic file streamer
//

template <typename T>
inline std::istringstream& operator>>(std::istringstream& is, T& v)
{
    getToken(is) >> v;
    return is;
}

// Note:
// By default << is defined only for ostream with the signature
//    template <typename T> inline std::ostream& operator<<(std::ostream& os, const T& v)
// so when we write:
//    os << "hi" << d;
// since I do not porvide an explicit overload for (const char *) the result of (os << "hi")
// is an ostream even if os is an ofstream, and all overloads I provide for ofstream
// would not be considererd
template <typename T>
inline std::ofstream& operator<<(std::ofstream& os, const T& v)
{
    static_cast<std::ostream&>(os) << v;
    return os;
}


//
// Vector streamer overloads
//

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
inline std::istringstream& operator>>(std::istringstream& is, std::vector<T,A>& v)
{
    size_t sz;
    is >> sz; // read size (this will call getToken)
    v.resize(sz);
    for(size_t i = 0; i < sz; ++i)
        is >> v[i];  // read i-th value (this will call getToken)
    getToken(is);
    return is;
}


//
// Date streamer overloads
//

inline std::ofstream& operator<<(std::ofstream& os, const Date& d)
{
    os << d.to_string(false);
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Date& d)
{
    os << d.to_string(true);
    return os;
}

inline std::istringstream& operator>>(std::istringstream& is, Date& v)
{
    string tmp;
    getToken(is) >> tmp;
    unsigned y = std::atoi(tmp.substr(0,4).c_str());
    unsigned m = std::atoi(tmp.substr(4,2).c_str());
    unsigned d = std::atoi(tmp.substr(6,2).c_str());
    v.init(y,m,d);
    return is;
}

