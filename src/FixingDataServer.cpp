#include <regex>
#include <limits>
#include "FixingDataServer.h"
#include "Macros.h"
#include "Streamer.h"

namespace minirisk {

// Populate the fixing data server with the data provided in the file fixings.txt.
FixingDataServer::FixingDataServer(const string& filename)
    {
        std::ifstream is(filename); // has to input the corret filename, or we display error message
		MYASSERT(!is.fail(), "Could not open file " << filename);
		do {
            string name;
            string fixingdate;
			double value;
			is >> name >> fixingdate >> value;
			
            if (name == "") {break;} // break out if input 'name' is empty
            
			Date fxdate(fixingdate);
			auto ins_name = m_data.find(name);
			
            if (ins_name == m_data.end()) // if we can not match in existing data sever, insert this new currency price value together with its 'name' & 'Date'
			{
				std::map<Date, double> tmpmap;
				tmpmap.emplace(fxdate, value);
				m_data.emplace(name, tmpmap);
			}
			else
			{
				auto ins_date = ins_name->second.emplace(fxdate, value);
                // if input string & name both exist, display message of this case
				MYASSERT(ins_date.second, "Duplicated risk factor: " << name << Date(fixingdate).to_string());
			}
		} while (is);
	}
 
// return the fixing if available, otherwise trigger an error
double FixingDataServer::get(const string& name,const Date&t) const
    {
        auto iter_name = m_data.find(name);
		MYASSERT(iter_name != m_data.end(), "Fixing not found: " << name);
		auto iter_date = iter_name->second.find(t);
		MYASSERT(iter_date != iter_name->second.end(), "Fixing not found: " << name << "," << t.to_string());
		return iter_date->second;
	}

// return the fixing if available, NaN otherwise, and set the flag if found
std::pair<double, bool> FixingDataServer::lookup(const string& name, const Date&t) const
	{
		auto iter_name = m_data.find(name);
		if (iter_name == m_data.end())
		{
			return (std::make_pair(std::numeric_limits<double>::quiet_NaN(), false)); // return NaN if could not find value
		}
		else
		{
			auto iter_date = iter_name->second.find(t);
			return (iter_date != iter_name->second.end())  // return value if it could be found
				? std::make_pair(iter_date->second, true)
				: std::make_pair(std::numeric_limits<double>::quiet_NaN(), false);
		}
	}

} // namespace minirisk

