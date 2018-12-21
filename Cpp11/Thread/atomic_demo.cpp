#include <atomic>
#include <iostream>

using namespace std;

struct Foo {
    Foo() : counter_(0) {}

    std::atomic_int counter_;
};

int main()
{
    std::atomic_uint i(0);
    Foo foo;
    // std::atomic_uint i = 0;

    // uint32_t j = ++i;
    uint32_t j = i++;

    cout << "j = " << j << endl;
    cout << "i = " << i << endl;

    if( i == 1 )
        cout << "i is 1" << endl;

    return 0;
}

