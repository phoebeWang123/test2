#pragma once

#include "Macros.h"
#include <string>
#include <array>

namespace minirisk {

	struct Date
	{
	public:
		static const unsigned first_year = 1900;
		static const unsigned last_year = 2200;
		static const unsigned n_years = last_year - first_year;

	private:
		static std::string padding_dates(unsigned);

		friend long operator-(const Date& d1, const Date& d2);

		static const std::array<unsigned, 12> days_in_month;  // num of days in month M in a normal year
		static const std::array<unsigned, 12> days_ytd;      // num of days since 1-jan to 1-M in a normal year
		static const std::array<unsigned, n_years> days_epoch;   // num of days since 1-jan-1900 to 1-jan-yyyy (until 2200)

	public:
		// Default constructor
		Date() :m_serial(0)
		{}

		void init(unsigned &serial)
		{
			m_serial = serial;
		}

		// Constructor where the input value is checked.
		Date(unsigned year, unsigned month, unsigned day)
		{
			init(year, month, day);
		}

		void init(unsigned year, unsigned month, unsigned day)
		{
			check_valid(year, month, day);
			m_serial = days_epoch[year - first_year] + days_ytd[month - 1] + ((month > 2 && is_leap_year(year)) ? 1 : 0) + (day - 1);
		}
		static bool is_valid_date(unsigned y, unsigned m, unsigned d) {

			unsigned dmax = days_in_month[m - 1] + ((m == 2 && is_leap_year(y)) ? 1 : 0);
			return y >= first_year && y < last_year&&m >= 1 && m <= 12 && d >= 1 && d <= dmax;
		}

		void to_y_m_d(unsigned *y_o, unsigned *m_o, unsigned *d_o) {
			*y_o = year();
			bool leap = is_leap_year(*y_o);
			*m_o = month(*y_o, leap);
			*d_o = day(*y_o, *m_o, leap);
		}

		unsigned serial() {
			return m_serial;
		}

		static void check_valid(unsigned y, unsigned m, unsigned d);



		bool operator<(const Date& d) const
		{
			return m_serial < d.m_serial;
		}

		bool operator==(const Date& d) const
		{
			return m_serial == d.m_serial;
		}

		bool operator>(const Date& d) const
		{
			return d < (*this);
		}

		static bool is_leap_year(unsigned yr);

		unsigned year() const;
		unsigned month(unsigned y, bool leap) const;
		unsigned day(unsigned y, unsigned m, bool leap) const;

		// In YYYYMMDD format
		std::string to_string(bool pretty = true) const
		{
			unsigned y = year();
			bool leap = is_leap_year(y);
			unsigned m = month(y, leap);
			unsigned d = day(y, m, leap);
			return pretty
				? std::to_string((int)d) + "-" + std::to_string((int)m) + "-" + std::to_string(y)
				: std::to_string(m_serial);
		}
		void init_reversal(unsigned serial)
		{
			m_serial = (unsigned)serial;
		}

	private:
		unsigned m_serial;      // number of days since 1-Jan-1900
	};

	long operator-(const Date& d1, const Date& d2);

	inline double time_frac(const Date& d1, const Date& d2)
	{
		return static_cast<double>(d2 - d1) / 365.0;
	}

} // namespace minirisk
