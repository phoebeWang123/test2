#pragma once

#include <map>
#include <regex>
#include "Global.h"
#include "Date.h"

namespace minirisk {

// Build struct FixingDataServer for past observations
struct FixingDataServer
{
public:
    FixingDataServer(const string& filename);
    double get(const string& name, const Date& t) const; // return the fixing if available, otherwise trigger an error
    std::pair<double, bool> lookup(const string& name, const Date& t) const; // return the fixing if available, NaN otherwise, and set the flag if found
    
private:
    std::map<string, std::map<Date,double>> m_data; // use string & date as matching key to search for data
};
    
} // namespace minirisk





