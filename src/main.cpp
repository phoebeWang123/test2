#include <iostream>
#include <memory>

using namespace std;

#include "TradePayment.h"

typedef std::shared_ptr<ITrade> ptrade_t;
typedef std::vector<ptrade_t> portfolio_t;

int main()
{
    portfolio_t p;

    TradePayment pmt;
    pmt.init("USD", 10, Date(2020,2,1));
    p.push_back(ptrade_t(new TradePayment(pmt)));

    cout << "Underlying: " << p[0]->underlyings()[0] << endl;
    cout << "Quantity: " << p[0]->quantity() << endl;

    return 0;
}
