#include "FixingDataServer.h"
#include "Macros.h"
#include "Streamer.h"
#include <regex>
#include <limits>

namespace minirisk{
    
//definition of the consrtuctor
//Populate the fixing data server with the data provided in the file fix-ings.txt.
FixingDataServer::FixingDataServer(const string& name)
{
    std::ifstream is(name);// if input is not the right name, error massege appear
    MYASSERT(!is.fail(), "Wrong filename"<<name<<std::endl;)
    while (is) {
        string m_name;
        string m_date;
        double value;
        is>>m_name>>m_date>>value;
        
        if (m_name=="") {
            break;
        }
        
        Date fixed_date(m_date);
        auto exist_name=m_data.find(m_name); //using name as the key word to search
        if(exist_name !=m_data.end()) //if the name is existing we should double check the ccy date to ensure if we should insert it or not
        {
            auto exist_date=exist_name->second.emplace(m_date,value);
            string cout_date= fixed_date.to_string();
            MYASSERT(exist_date.second, cout_date);
            
        }
        else//if we cannot find the name in the existing data server, we insert it into the pool with its information
        {
            std::map<Date, double> insertmap;
            insertmap.emplace(m_date,value);
            m_data.emplace(m_name,insertmap);
        }
    }
}

// return the fixing if available, otherwise trigger an error
double FixingDataServer::get(const string& name, const Date& date) const
    {
        auto name_itr=m_data.find(name);
        MYASSERT(name_itr!=m_data.end(), "Cannot find the Fixing Rate: "<<name);
        auto date_itr=name_itr->second.find(date);
        MYASSERT(date_itr!=name_itr->second.end(), "Cannot find the Fixing Rate: "<<name<<"."<<date.to_string())
        return date_itr->second; // if the name and date both match, return to the rate value
        
    }

// return the fixing if available, NaN otherwise, and set the flag if found
std::pair<double ,bool> FixingDataServer::lookup(const string& name, const Date& t) const
    {
        auto name_itr=m_data.find(name);
        if (name_itr==m_data.end()) {
            auto result=std::make_pair(std::numeric_limits<double>::quiet_NaN(), false);
            return result;
        }
        else
        {
            auto date_itr=name_itr->second.find(t);
            if (date_itr!=name_itr->second.end()) {
                return (std::make_pair(date_itr->second, true));
            }
            else
                return (std::make_pair(std::numeric_limits<double>::quiet_NaN(), false));
        }
    }
    

}
