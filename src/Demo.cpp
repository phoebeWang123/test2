#include <iostream>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <memory>

using namespace std;

#include "TradePayment.h"
#include "MarketDataServer.h"
#include "PortfolioUtils.h"

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

void print_portfolio(const portfolio_t& portfolio)
{
    std::for_each(portfolio.begin(), portfolio.end(), [](const ptrade_t& pt){ pt->print(std::cout); });
}

double total(const std::vector<double>& values)
{
    return std::accumulate(values.begin(), values.end(), 0.0);
}

int main()
{
    const char *fn = "portfolio.txt";

    // initialize market data server
    std::shared_ptr<const MarketDataServer> mds(new MarketDataServer);
    //std::cout << "FX.SPOT.EUR.USD: " << mkt->get("FX.SPOT.EUR.USD") << "\n";

    // create portfolio
    portfolio_t portfolio = createPortfolio();

    // display portfolio
    print_portfolio(portfolio);

    // test saving to file
    save_portfolio(fn, portfolio);

    // test reloading the portfolio
    portfolio = load_portfolio(fn);

    // display portfolio
    print_portfolio(portfolio);

    // convenience variables
    const size_t n_trades = portfolio.size();

    // get pricers
    std::vector<ppricer_t> pricers(get_pricers(portfolio));

    // Init market object
    Date today(2017,8,5);
    Market mkt(mds, today);

    // Price all products. Market objects are automatically constructed on demand,
    // fetching data as needed from the market data server.
    {
        auto prices = compute_prices(pricers, mkt);
        std::cout << "Total book PV: " << total(prices) << " USD\n\n";
    }

    // disconnect the market (no more fetching from the market data server allowed)
    mkt.disconnect();

    // display all relevant risk factors
    {
        std::cout << "Risk factors: ";
        auto tmp = mkt.get_risk_factors("");
        std::for_each(tmp.begin(), tmp.end(), [](auto iter) {std::cout << iter.first << ", "; });
        std::cout << "\n\n";
    }

    {   // Compute PV01 (i.e. sensitivity with respect to interest rate dV/dr)
        std::vector<std::pair<string, std::vector<double>>> pv01(compute_pv01(pricers,mkt));  // PV01 per trade

        // display cumulated PV01 per currency
        std::cout << "PV01 (exposure per IR risk factor):\n";
        for (auto g = pv01.begin(); g != pv01.end(); ++g)
            std::cout << g->first << ": " << total(g->second) << " USD\n";
        std::cout << "\n";
    }

    return 0;
}
