#include <cstdio>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

int getN()
{
    cout << "global getN" << endl;
    return 5;
}

struct Test {
    Test() : n(getN()) 
    { cout << "Test constructor " << n << endl; }

    int n;
};


Test gTest;
static const int n_dummy = getN();

int main()
{
    cout << "main started" << endl;
    return 0;
}











