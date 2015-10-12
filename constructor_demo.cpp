#include <cstdio>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

class Base {
public:
    Base( int i )
    { cout << "Base constructor " << i << endl; }
};


class Derived : public Base {
public:
    Derived( int i ) : Base(i)          //!! NOTE
    { cout << "Derived constructor " << i << endl; }
};


int main()
{
    Derived d(5);
    
    return 0;
}











