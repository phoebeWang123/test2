#include <iostream>
#include <algorithm>
#include <numeric>
#include <fstream>

using namespace std;

#include "TradeLoader.h"
#include "MarketDataServer.h"

portfolio_t createPortfolio()
{
    portfolio_t portfolio;

    TradePayment pmt;

    pmt.init("USD", 10, Date(2020,2,1));
    portfolio.push_back(ptrade_t(new TradePayment(pmt)));

    pmt.init("EUR", 20, Date(2020,2,2));
    portfolio.push_back(ptrade_t(new TradePayment(pmt)));

    return portfolio;
}

void printPortfolio(const portfolio_t& portfolio)
{
    std::for_each(portfolio.begin(), portfolio.end(), [](const ptrade_t& pt){ pt->print(std::cout); });
}

std::vector<double> prices_per_trade(const std::vector<ppricer_t>& pricers, Market& mkt)
{
    std::vector<double> prices(pricers.size());
    std::transform(pricers.begin(), pricers.end(), prices.begin()
        , [&mkt](const ppricer_t &pp) -> double { return pp->price(mkt); });
    return prices;
}

double total(const std::vector<double>& values)
{
    return std::accumulate(values.begin(), values.end(), 0.0);
}

// FIXME: move this constant to Global.h and define it in Global.cpp
const string ir_rate_prefix = "IR.";


int main()
{
    const char *fn = "portfolio.txt";

    // initialize market data server
    const MarketDataServer *mds = MarketDataServer::instance();
    //std::cout << "FX.SPOT.EUR.USD: " << mkt->get("FX.SPOT.EUR.USD") << "\n";

    // create portfolio
    portfolio_t portfolio = createPortfolio();

    // display portfolio
    printPortfolio(portfolio);

    // test saving to file
    std::ofstream of(fn);
    std::for_each(portfolio.begin(), portfolio.end(), [&of](const ptrade_t &pt){ pt->save(of); });
    of.close();

    // reset portfolio
    portfolio.clear();

    // test reloading the portfolio
    std::ifstream is(fn);
    string tmp;
    while(true) {
        getline(is, tmp);
        if (is.eof())
            break;
        portfolio.push_back(load_trade(tmp));
    }

    // display portfolio
    printPortfolio(portfolio);

    // convenience variables
    const size_t n_trades = portfolio.size();

    // get pricers
    std::vector<ppricer_t> pricers(n_trades);
    std::transform( portfolio.begin(), portfolio.end(), pricers.begin()
                  , [](const ptrade_t &pt) -> ppricer_t { return pt->pricer(); } );

    // Init market object
    Date today(2017,8,5);
    Market mkt(mds, today);

    // Price all products. Market objects are automatically constructed on demand,
    // fetching data as needed from the market data server.
    {
        auto prices = prices_per_trade(pricers, mkt);
        std::cout << "Total book PV: " << total(prices) << " USD\n\n";
    }

    // disconnect the market (no more fetching from the market data server allowed)
    mkt.disconnect();

    // display all relevant risk factors
    {
        std::cout << "Risk factors: ";
        auto tmp = mkt.get_risk_factors("");
        std::for_each(tmp.begin(), tmp.end(), [](auto iter) {std::cout << iter.first << "; "; });
        std::cout << "\n\n";
    }

    //
    // Compute PV01 (i.e. sensitivity with respect to interest rate dV/dr)
    // Use central differences, absolute bump of 0.01%, rescale result for rate movement of 0.01%
    //
    {
        std::vector<std::pair<string, std::vector<double>>> pv01;  // PV01 per trade

        const double bump_size = 0.01 / 100;
        const double scaler = 0.01 / 100;

        // filter risk factors related to IR
        auto base = mkt.get_risk_factors(ir_rate_prefix);

        // compute prices for perturbated markets and aggregate results
        pv01.reserve(base.size());
        for (auto d = base.begin(); d != base.end(); ++d) {
            std::vector<double> pv_up, pv_dn;
            std::vector<std::pair<string, double>> bumped(1, *d);
            pv01.push_back(std::make_pair(d->first, std::vector<double>(n_trades)));

            // bump down and price
            bumped[0].second = d->second - bump_size;
            mkt.set_data_points(bumped);
            pv_dn = prices_per_trade(pricers, mkt);

            // bump up and price
            bumped[0].second = d->second + bump_size; // bump up
            mkt.set_data_points(bumped);
            pv_up = prices_per_trade(pricers, mkt);

            // reset market to its original state
            bumped[0].second = d->second;
            mkt.set_data_points(bumped);

            // compute estimator of the derivative via central finite differences
            std::transform(pv_up.begin(), pv_up.end(), pv_dn.begin(), pv01.back().second.begin()
                , [bump_size, scaler] (double hi, double lo)
                -> double { return (hi - lo) / (2.0 * bump_size) * scaler; });
        }

        // display cumulated PV01 per currency
        std::cout << "PV01 (exposure per IR risk factor):\n";
        for (auto g = pv01.begin(); g != pv01.end(); ++g)
            std::cout << g->first << ": " << total(g->second) << " USD\n";
        std::cout << "\n";
    }

    return 0;
}
