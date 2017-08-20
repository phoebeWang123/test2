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
    for (portfolio_t::const_iterator i = portfolio.begin(); i != portfolio.end(); ++i)
    {
        cout << "Id:         " << (*i)->id() << endl;
        cout << "Name:       " << (*i)->idname() << endl;
        cout << "Underlying: " << (*i)->underlyings()[0] << endl;
        cout << "Quantity:   " << (*i)->quantity() << endl;
    }
}

int main()
{
    portfolio_t portfolio = createPortfolio();

    printPortfolio(portfolio);

    // test saving to file
    std::ofstream of("portfolio.txt");
    std::for_each(portfolio.begin(), portfolio.end(), [&of](const ptrade_t &pt){ pt->save(of); });

    return 0;
}
