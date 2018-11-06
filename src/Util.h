#include <iostream>
#include <sstream> 



namespace minirisk {
	std::string extractTenor(std::string tenor) {
		unsigned first = tenor.find('.');
		unsigned last = tenor.find_last_of('.');
		std::string revisedTenor = tenor.substr(first + 1, last - first - 1);
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





	
}