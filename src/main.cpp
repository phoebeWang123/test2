#include <iostream>
#include <algorithm>
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

    // get pricers
    std::vector<ppricer_t> pricers(portfolio.size());
    std::transform( portfolio.begin(), portfolio.end(), pricers.begin()
                  , [](const ptrade_t &pt) -> ppricer_t { return pt->pricer(); } );

    // price all products
    Date today(2017,8,5);
    Market mkt(mds, today);
    std::vector<double> prices(portfolio.size());
    std::transform( pricers.begin(), pricers.end(), prices.begin()
                  , [&mkt](const ppricer_t &pp) -> double { return pp->price(mkt); } );

    return 0;
}
