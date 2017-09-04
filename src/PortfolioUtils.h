#pragma once

#include <vector>

#include "ITrade.h"
#include "IPricer.h"

struct Market;

// get pricer for each trade
std::vector<ppricer_t> get_pricers(const std::vector<ptrade_t>& portfolio);

// compute prices
std::vector<double> compute_prices(const std::vector<ppricer_t>& pricers, Market& mkt);

// Compute PV01 (i.e. sensitivity with respect to interest rate dV/dr)
// Use central differences, absolute bump of 0.01%, rescale result for rate movement of 0.01%
std::vector<std::pair<string, std::vector<double>>> compute_pv01(const std::vector<ppricer_t>& pricers, Market& mkt);

// save portfolio to file
void save_portfolio(const string& filename, const std::vector<ptrade_t>& portfolio);

// load portfolio from file
std::vector<ptrade_t>  load_portfolio(const string& filename);
