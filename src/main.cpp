#include <iostream>
#include <memory>
#include <algorithm>
#include <fstream>

using namespace std;

#include "TradePayment.h"

typedef std::shared_ptr<ITrade> ptrade_t;
typedef std::vector<ptrade_t> portfolio_t;

portfolio_t createPortfolio()
{
    portfolio_t portfolio;

    TradePayment pmt;

    pmt.init("USD", 10, Date(2020,2,1));
    portfolio.push_back(ptrade_t(new TradePayment(pmt)));

    pmt.init("USD", 20, Date(2020,2,1));
    portfolio.push_back(ptrade_t(new TradePayment(pmt)));

    return portfolio;
}

void printPortfolio(const portfolio_t& portfolio)
{
    std::for_each(portfolio.begin(), portfolio.end(), [](const ptrade_t& pt){ pt->print(std::cout); });
}

int main()
{
    // create portfolio
    portfolio_t portfolio = createPortfolio();

    // display portfolio
    printPortfolio(portfolio);

    // test saving to file
    std::ofstream of("portfolio.txt");
    std::for_each(portfolio.begin(), portfolio.end(), [&of](const ptrade_t &pt){ pt->save(of); });
    of.close();

    // reset portfolio
    portfolio.clear();

    // test reloading the portfolio
    // FIXME: implement

    // display portfolio
    printPortfolio(portfolio);

    return 0;
}
