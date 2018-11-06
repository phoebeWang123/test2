#include "CurveDiscount.h"
#include "Streamer.h"
#include "Market.h"


#include <cmath>

namespace minirisk {
	std::string extractTenors(std::string tenor) {
		unsigned first = tenor.find('.');
		unsigned last = tenor.find_last_of('.');
		std::string revisedTenor = tenor.substr(first + 1, last - first - 1);
		return revisedTenor;
	}
	int getNumDay(std::string numDay) {
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

	std::vector<std::tuple<string, double, double, double, double>> getRateMap(Market *mkt, const string& curve_name) {
		string curr = curve_name.substr(ir_curve_discount_prefix.length(), 3);
		string expr = "IR.[[:digit:]]+[A-Z]." + curr;
		std::vector<std::string> tenorVector = mkt->getTenors(expr);
		std::vector<std::tuple<string, double, double, double, double>> tenorMap;
		for (std::vector<std::string>::iterator it = tenorVector.begin(); it != tenorVector.end(); ++it) {
			std::string tenor = *it;			
			tenor = extractTenors(tenor) + "." + curr;
			double tenorRate = mkt->get_yield(tenor);
			double date = 1.0*getNumDay(extractTenors(*it));
			if (it != tenorVector.end() - 1) {
				std::string nextTenor = *(it + 1);
				nextTenor = extractTenors(nextTenor) + "." + curr;
				double nextTenorRate = mkt->get_yield(nextTenor);
				double nextDate = 1.0*getNumDay(extractTenors(*(it+1)));
				tenorMap.push_back(std::make_tuple(*it,date, tenorRate,nextDate,nextTenorRate));
			}
			else {
				tenorMap.push_back(std::make_tuple(*it, date, tenorRate, 0.0, 0.0));
			}						
		}
		return tenorMap; 
	}
	
	bool binary_search(const std::vector<std::tuple<string, double, double, double, double>>& sorted_vec, double key) {
		size_t mid, left = 0;
		size_t right = sorted_vec.size(); // one position passed the right end
		while (left < right) {
			mid = left + (right - left) / 2;
			if (key > std::get<1>(sorted_vec[mid])) {
				left = mid + 1;
			}
			else if (key < std::get<1>(sorted_vec[mid])) {
				right = mid;
			}
			else {
				return mid;
			}
		}

		return left;
	}

CurveDiscount::CurveDiscount(Market *mkt, const Date& today, const string& curve_name)
    : m_today(today)
    , m_name(curve_name)
	, m_rate(mkt->get_yield(curve_name.substr(ir_curve_discount_prefix.length(), 3)))
	, m_rateMap(getRateMap(mkt,curve_name))

{
} 


//border case NOT CONSIDERED YET!!!!! CHECK!
double  CurveDiscount::df(const Date& t) const
{
    MYASSERT((!(t < m_today)), "cannot get discount factor for date in the past: " << t);
    double dt = time_frac(m_today, t);
	int leftIndex = binary_search(m_rateMap, dt);
	double ti = std::get<1>(m_rateMap[leftIndex]);
	double ri = std::get<2>(m_rateMap[leftIndex]);
	double ti_plus_1 = std::get<3>(m_rateMap[leftIndex]);
	double ri_plus_1 = std::get<4>(m_rateMap[leftIndex]);
	double ri_to_ri_plus_1 = (ri_plus_1 * ti_plus_1 - ri * ti)/(ti_plus_1-ti);
   // return std::exp(-m_rate * dt);
	double df = std::exp(-ri * ti - ri_to_ri_plus_1 * (dt - ti));
	return df;
}

} // namespace minirisk
