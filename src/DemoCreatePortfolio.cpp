#include "TradePayment.h"
#include "PortfolioUtils.h"

using namespace minirisk;
int main(int argc, const char **argv)
{
    MYASSERT(argc > 1, "This demo requires the name of the file where the portfolio is to be saved to.");

    const char *filename = argv[1];

    // create a portfolio containing 2 payment trades
    portfolio_t portfolio;

    TradePayment pmt;

    pmt.init("USD", 10, Date(2020,2,1));
    portfolio.push_back(ptrade_t(new TradePayment(pmt)));

    pmt.init("EUR", 20, Date(2020,2,2));
    portfolio.push_back(ptrade_t(new TradePayment(pmt)));

    // display portfolio
    print_portfolio(portfolio);

    // save portfolio to file
    save_portfolio(filename, portfolio);

    return 0;
}

