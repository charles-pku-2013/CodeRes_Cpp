#include <iostream>
#include <iomanip>
#include <cmath>

int main() {
    using namespace std;

    float f = 0.33103448;
    cout << std::setprecision(100) << f << endl;
    double d = 0.33103448;
    cout << std::setprecision(100) << d << endl;

    return 0;
}

/*
 * 0.331034481525421142578125
 * 0.331034480000000019828121367027051746845245361328125
 */
