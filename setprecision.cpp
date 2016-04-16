#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>

using namespace std;

int main()
{
    float f1 = 1.0 / 3.0, f2 = 2.0 / 3.0, f3 = 1.0 / 6.0;
    int i = 1000000;

    cout << f1 << "\t" << f2 << "\t" << f3 << endl;
    cout << setprecision(2) << "hello " << i << "\t" << f1 << "\t" << f2 << "\t" << f3 << endl;

    // const long double pi = std::acos(-1.L);
    // std::cout << "default precision (6): " << pi << '\n'
              // << "std::precision(10):    " << std::setprecision(10) << pi << '\n'
              // << "max precision:         "
              // << std::setprecision(std::numeric_limits<long double>::digits10 + 1)
              // << pi << '\n';

    return 0;
}

/*
 * default precision (6): 3.14159
 * std::precision(10):    3.141592654
 * max precision:         3.141592653589793239
 */
