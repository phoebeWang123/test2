#include <iostream>

using namespace std;

#include "TradePayment.h"

int main()
{
    TradePayment p;
    p.init("USD", 10, Date(2020,2,1));

    cout << "Underlying: " << p.underlyings()[0] << endl;
    cout << "Quantity: " << p.quantity() << endl;

    return 0;
}
