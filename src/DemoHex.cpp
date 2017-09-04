#include <iostream>
#include <iomanip>

using namespace std;

void out_char_as_hex(int c)
{
    cout << hex << setw(2) << setfill('0') << c;
}

int main()
{
    double x = -0.15625;
    unsigned char *c = reinterpret_cast<unsigned char *>(&x);
    for (unsigned i = 0; i < 8; ++i)
        out_char_as_hex(c[i]);
    cout << endl;
    return 0;
}
