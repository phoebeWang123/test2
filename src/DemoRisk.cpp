#include <iostream>
#include <algorithm>

#include "MarketDataServer.h"
#include "PortfolioUtils.h"

using namespace::minirisk;
int main(int argc, const char **argv)
{
    if(argc != 3) {
        std::cout << "This demo requires the name of the file where the portfolio is to read from and the name of the file containing market data.\n"
                  << "Example:\n"
                  << "DemoRisk portfolio.txt risk_factors.txt\n";
        return -1;
    }

    // filenames
    const char *portfolio_file    = argv[1];
    const char *risk_factors_file = argv[2];

    // load the portfolio from file
    portfolio_t portfolio = load_portfolio(portfolio_file);

    // display portfolio
    print_portfolio(portfolio);

    // get pricers
    std::vector<ppricer_t> pricers(get_pricers(portfolio));

    // initialize market data server
    std::shared_ptr<const MarketDataServer> mds(new MarketDataServer(risk_factors_file));
    //std::cout << "FX.SPOT.EUR.USD: " << mkt->get("FX.SPOT.EUR.USD") << "\n";

    // Init market object
    Date today(2017,8,5);
    Market mkt(mds, today);

    // Price all products. Market objects are automatically constructed on demand,
    // fetching data as needed from the market data server.
    {
        auto prices = compute_prices(pricers, mkt);
        std::cout << "Total book PV: " << portfolio_total(prices) << " USD\n\n";
    }

    // disconnect the market (no more fetching from the market data server allowed)
    mkt.disconnect();

    // display all relevant risk factors
    {
        std::cout << "Risk factors: ";
        auto tmp = mkt.get_risk_factors("");
        std::for_each(tmp.begin(), tmp.end(), [](auto& iter) {std::cout << iter.first << ", "; });
        std::cout << "\n\n";
    }

    {   // Compute PV01 (i.e. sensitivity with respect to interest rate dV/dr)
        std::vector<std::pair<string, portfolio_values_t>> pv01(compute_pv01(pricers,mkt));  // PV01 per trade

        // display cumulated PV01 per currency
        std::cout << "PV01 (exposure per IR risk factor):\n";
        for (auto g = pv01.begin(); g != pv01.end(); ++g)
            std::cout << g->first << ": " << portfolio_total(g->second) << " USD\n";
        std::cout << "\n";
    }

    return 0;
}

