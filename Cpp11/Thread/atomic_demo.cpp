#include <atomic>
#include <iostream>

using namespace std;

int main()
{
    std::atomic_uint i(0);

    uint32_t j = ++i;

    cout << "j = " << j << endl;
    cout << "i = " << i << endl;

    if( i == 1 )
        cout << "i is 1" << endl;

    return 0;
}

