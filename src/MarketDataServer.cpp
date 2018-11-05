#include "MarketDataServer.h"
#include "Macros.h"
#include "Streamer.h"

#include <limits>
#include <algorithm> 
#include <vector>

namespace minirisk {

// transforms FX.SPOT.EUR.USD into FX.SPOT.EUR
string mds_spot_name(const string& name)
{
    // NOTE: in a real system error checks should be stricter, not just on the last 3 characters
    MYASSERT((name.substr(name.length() - 3, 3) == "USD"),
        "Only FX pairs in the format FX.SPOT.CCY.USD can be queried. Got " << name);
    return name.substr(0, name.length() - 4);
}

MarketDataServer::MarketDataServer(const string& filename) 
{
    std::ifstream is(filename);
    MYASSERT(!is.fail(), "Could not open file " << filename);
    do {
        string name;
        double value;
        is >> name >> value;
        //std::cout << name << " " << value << "\n";
        auto ins = m_data.emplace(name, value);
        MYASSERT(ins.second, "Duplicated risk factor: " << name);
    } while (is);
}

double MarketDataServer::get(const string& name) const
{
    auto iter = m_data.find(name);
    MYASSERT(iter != m_data.end(), "Market data not found: " << name);
    return iter->second;
}

std::pair<double, bool> MarketDataServer::lookup(const string& name) const
{
    auto iter = m_data.find(name);
    return (iter != m_data.end())  // found?
            ? std::make_pair(iter->second, true)
            : std::make_pair(std::numeric_limits<double>::quiet_NaN(), false);
}


std::string extractTenor(std::string tenor) {
	unsigned first = tenor.find('.');
	unsigned last = tenor.find_last_of('.');
	std::string revisedTenor = tenor.substr(first+1, last - first-1);
	return revisedTenor;
}


int getNumDays(std::string numDay) {
	char unit = numDay.back();
	int unitDay;
	if (unit == 'Y') {
		unitDay = 365;
	}
	else if (unit == 'M') {
		unitDay = 30;
	}
	else if (unit == 'W') {
		unitDay = 7;
	}
	else if (unit == 'D') {
		unitDay = 1;
	}
	else {
		//exception
	}
	int numOfUnit = std::stoi(numDay.substr(0, numDay.length() - 1));
	return unitDay * numOfUnit;
}


bool sortTenor(std::string &tenor1, std::string &tenor2) {
	std::string revisedTenor1 = extractTenor(tenor1);
	std::string revisedTenor2 = extractTenor(tenor2);
	int numDays1 = getNumDays(revisedTenor1);
	int numDays2 = getNumDays(revisedTenor2);
	return (numDays1 < numDays2);
}

//return sorted vector of tenors
std::vector<std::string> MarketDataServer::match(const std::string& expr) const
{
    std::regex r(expr);
	std::vector<std::string> result;
	for (auto it = m_data.begin(); it != m_data.end(); ++it) {
		string name = it->first;
		if (std::regex_match(name, r)) {
			result.push_back(name);
		}
	}
	std::sort(result.begin(), result.end(), sortTenor);
	return result;
}

} // namespace minirisk

