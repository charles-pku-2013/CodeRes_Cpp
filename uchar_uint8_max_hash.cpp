#include <iostream>
#include <limits>

using namespace std;

/*
 * NOTE!!! UCHAR_MAX = 255
 * size([0,255]) = 256
 * 用 char 做 hash，type a[UCHAR_MAX + 1]
 *
 * NOTE!!! 注意这个死循环
 * for (uint8_t i = 0; i <= UCHAR_MAX; ++i)
 *      do_sth;
 * 当 i == UCHAR_MAX 时，循环并未结束，再+1 又变回0，导致无限循环
 */

int main()
{
    cout << (uint32_t)std::numeric_limits<uint8_t>::max() << endl;
    cout << UCHAR_MAX << endl;
    return 0;
}

