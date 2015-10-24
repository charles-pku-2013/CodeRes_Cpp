#include <cstdio>
#include <iostream>

using namespace std;

// Compile error: Invalid constructor!

class A {
public:
    A( int n ) { value = n; }
    A( A other ) { value = other.value; }
    void print()
    { cout << value << endl; }
private:
    int value;
};


int main()
{
    A a = 10;
    A b = a;

    return 0;
}
