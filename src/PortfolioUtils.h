#pragma once

#include <vector>

#include "ITrade.h"
#include "IPricer.h"
#include "FixingDataServer.h"

namespace minirisk {

struct Market;

typedef std::vector<double> portfolio_values_t;
typedef std::vector<std::pair<double, string>> portfolio_values_tfails;
typedef std::vector<std::pair<double, string>> portfolio_values_tpairs;

// get pricer for each trade
std::vector<ppricer_t> get_pricers(const portfolio_t& portfolio);

// compute prices NEED TO CHECK! ATTENTION!
//portfolio_values_t compute_prices(const std::vector<ppricer_t>& pricers, Market& mkt);
portfolio_values_tpairs compute_prices(const std::vector<ppricer_t>& pricers, Market& mkt, FixingDataServer& fds);

// compute the cumulative book value	
std::pair<double, std::vector<std::pair<double, string>>> portfolio_total(const std::vector<std::pair<double, string>> values);

// Compute PV01 (i.e. sensitivity with respect to interest rate dV/dr)
// Use central differences, absolute bump of 0.01%, rescale result for rate movement of 0.01%
std::vector<std::pair<string, portfolio_values_t>> compute_pv01(const std::vector<ppricer_t>& pricers, const Market& mkt);

// Compute PV01 (i.e. sensitivity with respect to interest rate dV/dr)
// Use central differences, absolute bump of 0.01%, rescale result for rate movement of 0.01%
std::vector<std::pair<string, portfolio_values_tpairs>> compute_pv01_parallel(const std::vector<ppricer_t>& pricers, const Market& mkt, FixingDataServer& fds);
std::vector<std::pair<string, portfolio_values_tpairs>> compute_pv01_bucketed(const std::vector<ppricer_t>& pricers, const Market& mkt, FixingDataServer& fds);


// save portfolio to file
void save_portfolio(const string& filename, const std::vector<ptrade_t>& portfolio);

// load portfolio from file
std::vector<ptrade_t>  load_portfolio(const string& filename);

// print portfolio to cout
void print_portfolio(const portfolio_t& portfolio);

// print portfolio to cout
void print_price_vector(const string& name, const portfolio_values_tpairs& values);


} // namespace minirisk

