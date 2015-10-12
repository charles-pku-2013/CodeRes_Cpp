#include <cstdio>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

void func()
{
    struct A {
        A(int _a) : mA(_a) {}
        int mA;
    };
    
    A a(5);
    cout << a.mA << endl;
}

int main()
{
    func();
    return 0;
}











