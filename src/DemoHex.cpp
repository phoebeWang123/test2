#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    double x = 0.15625;
    unsigned char *c = reinterpret_cast<unsigned char *>(&x);
    cout << hex << (int) c[0] << hex << (int) c[1] << hex << (int) c[2] << hex << (int) c[3]
         << hex << (int) c[4] << hex << (int) c[5] << hex << (int) c[6] << hex << (int) c[7]
         << endl;
    return 0;
}
