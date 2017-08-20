#include <iostream>
#include <memory>
#include <algorithm>
#include <fstream>

using namespace std;

#include "TradePayment.h"

typedef std::shared_ptr<ITrade> ptrade_t;
typedef std::vector<ptrade_t> portfolio_t;

int main()
{
    portfolio_t portfolio;

    TradePayment pmt;

    pmt.init("USD", 10, Date(2020,2,1));
    portfolio.push_back(ptrade_t(new TradePayment(pmt)));

    pmt.init("USD", 20, Date(2020,2,1));
    portfolio.push_back(ptrade_t(new TradePayment(pmt)));

    // test saving to file
    std::ofstream of("portfolio.txt");

    std::for_each(portfolio.begin(), portfolio.end(), [&of](const ptrade_t &pt){ pt->save(of); });

    for (size_t i = 0; i < portfolio.size(); ++i)
    {
        cout << "Id:         " << portfolio[i]->id() << endl;
        cout << "Underlying: " << portfolio[i]->underlyings()[0] << endl;
        cout << "Quantity:   " << portfolio[i]->quantity() << endl;
    }

    return 0;
}
