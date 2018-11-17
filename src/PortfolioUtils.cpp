#include "Global.h"
#include "PortfolioUtils.h"
#include "TradePayment.h"

#include <numeric>
#include <set>

namespace minirisk {

static double num_errors; // add counter for pricing failures
void print_portfolio(const portfolio_t& portfolio)
{
    std::for_each(portfolio.begin(), portfolio.end(), [](auto& pt){ pt->print(std::cout); });
}

std::vector<ppricer_t> get_pricers(const portfolio_t& portfolio)
{
    std::vector<ppricer_t> pricers(portfolio.size());
    std::transform( portfolio.begin(), portfolio.end(), pricers.begin()
                  , [](auto &pt) -> ppricer_t { return pt->pricer(); } );
    return pricers;
}


//Modify function compute_prices to enable recover from pricing failures
portfolio_values_tpairs compute_prices(const std::vector<ppricer_t>& pricers, Market& mkt, FixingDataServer& fds)
{
	portfolio_values_tpairs pair_vector;
	//NEED TO CHECK PRICE FUNCTION!!!!!
	auto tmp_price = [&mkt, &fds](decltype(*(pricers.begin())) &pp) -> double { return pp->price(mkt); };
	for (auto iter = pricers.begin(); iter != pricers.end(); iter++)
	{
		try
		{
			double tmp_price_double = tmp_price(*iter);
			pair_vector.push_back(std::make_pair(tmp_price_double, ""));
		}
		catch (std::exception& s) // catch out error message instead of terminal the program
		{
			pair_vector.push_back(std::make_pair(std::numeric_limits<double>::quiet_NaN(), s.what()));
		}
	}
	return pair_vector;
}

// NEED TO CHECK!!!! ATTENTION
//portfolio_values_t compute_prices(const std::vector<ppricer_t>& pricers, Market& mkt)
//{
//    portfolio_values_t prices(pricers.size());
//    std::transform(pricers.begin(), pricers.end(), prices.begin()
//        , [&mkt](auto &pp) -> double { return pp->price(mkt); });
//    return prices;
//}
//
//double portfolio_total(const portfolio_values_t& values)
//{
//    return std::accumulate(values.begin(), values.end(), 0.0);
//}
std::pair<double, std::vector<std::pair<double, string>>> portfolio_total(const std::vector<std::pair<double, string>> values)
{
	double success_value_total = 0.0;
	size_t trade_num = 0;
	num_errors = 0;
	std::vector<std::pair<double, string>> error_message;

	for (auto iter = values.begin(); iter != values.end(); iter++)
	{
		trade_num += 1;
		if (!isnan(iter->first))
		{
			success_value_total += iter->first;
		}
		else
		{
			num_errors++;
			error_message.push_back(std::make_pair((double)trade_num, iter->second));
		}
	}
	auto portfolio_total_value = std::make_pair(success_value_total, error_message);
	return portfolio_total_value;
}
//NEED TO CHECK!!!! ATTENTION
//std::vector<std::pair<string, portfolio_values_t>> compute_pv01(const std::vector<ppricer_t>& pricers, const Market& mkt)
//{
//    std::vector<std::pair<string, portfolio_values_t>> pv01;  // PV01 per trade
//
//    const double bump_size = 0.01 / 100;
//
//    // filter risk factors related to IR
//    auto base = mkt.get_risk_factors(ir_rate_prefix + "[A-Z]{3}");
//
//    // Make a local copy of the Market object, because we will modify it applying bumps
//    // Note that the actual market objects are shared, as they are referred to via pointers
//    Market tmpmkt(mkt);
//
//    // compute prices for perturbated markets and aggregate results
//    pv01.reserve(base.size());
//    for (const auto& d : base) {
//        std::vector<double> pv_up, pv_dn;
//        std::vector<std::pair<string, double>> bumped(1, d);
//        pv01.push_back(std::make_pair(d.first, std::vector<double>(pricers.size())));
//
//        // bump down and price
//        bumped[0].second = d.second - bump_size;
//        tmpmkt.set_risk_factors(bumped);
//        pv_dn = compute_prices(pricers, tmpmkt);
//
//        // bump up and price
//        bumped[0].second = d.second + bump_size; // bump up
//        tmpmkt.set_risk_factors(bumped);
//        pv_up = compute_prices(pricers, tmpmkt);
//
//
//        // restore original market state for next iteration
//        // (more efficient than creating a new copy of the market at every iteration)
//        bumped[0].second = d.second;
//        tmpmkt.set_risk_factors(bumped);
//
//        // compute estimator of the derivative via central finite differences
//        double dr = 2.0 * bump_size;
//        std::transform(pv_up.begin(), pv_up.end(), pv_dn.begin(), pv01.back().second.begin()
//            , [dr](double hi, double lo) -> double { return (hi - lo) / dr; });
//    }
//
//    return pv01;
//}

// 2 different risk functions, PV01Bucketed computes risk with respect to individual yield curves
std::vector<std::pair<string, portfolio_values_tpairs>> compute_pv01_bucketed(const std::vector<ppricer_t>& pricers, const Market& mkt, FixingDataServer& fds)
{
	std::vector<std::pair<string, portfolio_values_tpairs>> pv01; // PV01 per trade
	const double bump_size = 0.01 / 100;
	num_errors = 0;

	// filter risk factors related to IR
	auto base = mkt.get_risk_factors(ir_rate_prefix + "(.*)." + "[A-Z]{3}");
	// Make a local copy of the Market object, because we will modify it applying bumps
	// Note that the actual market objects are shared, as they are referred to via pointers
	Market tmpmkt(mkt);
	pv01.reserve(base.size());
	// compute prices for perturbated markets and aggregate results
	for (size_t i = 0; i < base.size(); i++) {
		portfolio_values_tpairs pv_up, pv_dn;
		std::vector<std::pair<string, double>> bumped_up = base, bumped_down = base;
		pv01.push_back(std::make_pair(string("bucketed ") + base[i].first, portfolio_values_tpairs(pricers.size())));

		// bump up & down then price
		bumped_up[i] = std::make_pair(base[i].first, base[i].second + bump_size); // bump up
		bumped_down[i] = std::make_pair(base[i].first, base[i].second - bump_size);

		tmpmkt.set_risk_factors(bumped_up);
		pv_up = compute_prices(pricers, tmpmkt, fds);
		tmpmkt = mkt;
		tmpmkt.set_risk_factors(bumped_down);
		pv_dn = compute_prices(pricers, tmpmkt, fds);
		tmpmkt = mkt;

		// compute estimator of the derivative via central finite differences
		double dr = 2.0 * bump_size;
		for (size_t j = 0; j < pricers.size(); j++)
		{
			if (isnan(pv_up[j].first) || isnan(pv_dn[j].first)) {
				num_errors++;
				pv01.back().second[j] = std::make_pair(std::numeric_limits<double>::quiet_NaN(), isnan(pv_up[j].first) ? pv_up[j].second : pv_dn[j].second);
			}
			else {
				pv01.back().second[j] = std::make_pair((pv_up[j].first - pv_dn[j].first) / dr, string(""));
			}
		}
	}
	return pv01;
}

// PV01_parallel computes risk with respect to parallel shift of the yield curve for one currency
std::vector<std::pair<string, portfolio_values_tpairs>> compute_pv01_parallel(const std::vector<ppricer_t>& pricers, const Market& mkt, FixingDataServer& fds)
{
	std::vector<std::pair<string, portfolio_values_tpairs>> pv01;  // PV01 per trade
	const double bump_size = 0.01 / 100;
	num_errors = 0;

	// filter risk factors related to IR
	auto base = mkt.get_risk_factors(ir_rate_prefix + "(.*)." + "[A-Z]{3}");

	std::set<std::string> ccynames;
	for (auto d : base) {
		ccynames.emplace(d.first.substr(d.first.length() - 3, 3));
	}

	// Make a local copy of the Market object, because we will modify it applying bumps
	// Note that the actual market objects are shared, as they are referred to via pointers
	Market tmpmkt(mkt);

	// compute prices for perturbated markets and aggregate results
	pv01.reserve(base.size());
	for (auto ccyname : ccynames) {
		portfolio_values_tpairs pv_up, pv_dn;
		std::vector<std::pair<string, double>> bumped_up;
		std::vector<std::pair<string, double>> bumped_down;
		pv01.push_back(std::make_pair(string("parallel " + string("IR.") + ccyname), portfolio_values_tpairs(pricers.size())));
		std::regex r(ir_rate_prefix + "(.*)." + ccyname);
		std::smatch m_smatch;

		for (auto iter_base = base.begin(); iter_base != base.end(); iter_base++)
		{
			if (std::regex_search(iter_base->first, m_smatch, r))
			{
				bumped_up.push_back(std::make_pair(iter_base->first, (iter_base->second) + bump_size));
				bumped_down.push_back(std::make_pair(iter_base->first, (iter_base->second) - bump_size));
			}
		}
		tmpmkt.set_risk_factors(bumped_up);
		pv_up = compute_prices(pricers, tmpmkt, fds);
		tmpmkt.set_risk_factors(bumped_down);
		pv_dn = compute_prices(pricers, tmpmkt, fds);
		tmpmkt = mkt;

		// compute estimator of the derivative via central finite differences
		double dr = 2.0 * bump_size;
		for (size_t i = 0; i < pricers.size(); i++)
		{
			if (isnan(pv_up[i].first) || isnan(pv_dn[i].first)) {
				num_errors++;
				pv01.back().second[i] = std::make_pair(std::numeric_limits<double>::quiet_NaN(), isnan(pv_up[i].first) ? pv_up[i].second : pv_dn[i].second);
			}
			else {
				pv01.back().second[i] = std::make_pair((pv_up[i].first - pv_dn[i].first) / dr, string(""));
			}
		}
	}
	return pv01;
}

// Add another function for Greeks calculation
std::vector<std::pair<string, portfolio_values_tpairs>> fx_delta(const std::vector<ppricer_t>& pricers, const Market& mkt, FixingDataServer& fds)
{
	std::vector<std::pair<string, portfolio_values_tpairs>> fxdelta;
	const double rel_bump_size = 0.1 / 100; // use bump size of 0.1/100

	auto base = mkt.get_risk_factors(fx_spot_prefix + "[A-Z]{3}");

	Market tmpmkt(mkt);
	fxdelta.reserve(base.size());

	for (size_t i = 0; i < base.size(); i++)
	{
		portfolio_values_tpairs fx_up, fx_dn;
		std::vector<std::pair<string, double>> bumped_up = base, bumped_down = base;
		fxdelta.push_back(std::make_pair(string("FX delta ") + base[i].first, portfolio_values_tpairs(pricers.size())));

		const double bump_size = base[i].second * rel_bump_size;
		bumped_up[i] = std::make_pair(base[i].first, base[i].second + bump_size);
		bumped_down[i] = std::make_pair(base[i].first, base[i].second - bump_size);

		tmpmkt.set_risk_factors(bumped_up);
		fx_up = compute_prices(pricers, tmpmkt, fds);
		tmpmkt.set_risk_factors(bumped_down);
		fx_dn = compute_prices(pricers, tmpmkt, fds);
		tmpmkt = mkt;

		// use central difference with a relative bump of 0.1
		double dr = 2.0 * bump_size;
		for (size_t j = 0; j < pricers.size(); j++)
		{
			if (isnan(fx_up[j].first) || isnan(fx_dn[j].first)) {
				fxdelta.back().second[j] = std::make_pair(std::numeric_limits<double>::quiet_NaN(), isnan(fx_up[j].first) ? fx_up[j].second : fx_dn[j].second);
			}
			else {
				fxdelta.back().second[j] = std::make_pair((fx_up[j].first - fx_dn[j].first) / dr, string(""));
			}
		}
	}
	return fxdelta;
}

ptrade_t load_trade(my_ifstream& is)
{
    string name;
    ptrade_t p;

    // read trade identifier
    guid_t id;
    is >> id;

    if (id == TradePayment::m_id)
        p.reset(new TradePayment);
    else
        THROW("Unknown trade type:" << id);

    p->load(is);

    return p;
}

void save_portfolio(const string& filename, const std::vector<ptrade_t>& portfolio)
{
    // test saving to file
    my_ofstream of(filename);
    for( const auto& pt : portfolio) {
        pt->save(of);
        of.endl();
    }
    of.close();
}

std::vector<ptrade_t> load_portfolio(const string& filename)
{
    std::vector<ptrade_t> portfolio;

    // test reloading the portfolio
    my_ifstream is(filename);
    while (is.read_line())
        portfolio.push_back(load_trade(is));

    return portfolio;
}

//ATTENTION: NEED TO CHECK!
//void print_price_vector(const string& name, const portfolio_values_t& values)
//{
//    std::cout
//        << "========================\n"
//        << name << ":\n"
//        << "========================\n"
//        << "Total: " << portfolio_total(values)
//        << "\n========================\n";
//
//    for (size_t i = 0, n = values.size(); i < n; ++i)
//        std::cout << std::setw(5) << i << ": " << values[i] << "\n";
//
//    std::cout << "========================\n\n";
//}

// display both the total for the book and the number of trades which fail to price.
void print_price_vector(const string& name, const portfolio_values_tpairs& values)
{
	auto r = portfolio_total(values);
	std::cout
		<< "========================\n"
		<< name << ":\n"
		<< "========================\n"
		<< "Total:  " << r.first
		<< "\n"
		<< "Errors: " << num_errors
		<< "\n\n========================\n";
	std::vector<double> col1;
	std::vector<string> col2;
	for (auto iterc : values)
	{
		col1.push_back(iterc.first);
		col2.push_back(iterc.second);
	}

	for (size_t i = 0; i < values.size(); i++)
	{
		if (isnan(col1[i])) {
			std::cout << std::setw(5) << i << ": " << col2[i] << "\n";
		}
		else {
			std::cout << std::setw(5) << i << ": " << col1[i] << "\n";
		}
	}
	std::cout << "========================\n\n";
}


} // namespace minirisk
