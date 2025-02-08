#include <iostream>
using namespace std;

class C
{
    int m;
public:
    C(){cout<<"in C constructor"<<endl;}
    ~C(){cout<<"in C destructor"<<endl;}
};

class A
{
public:
    A(){cout<<"in A constructor"<<endl;}
    ~A(){cout<<"in A destructor"<<endl;}
};

class B:public A
{
public:
    C c;
    char* resource;

    B()
    {
        resource=new char[100];
        cout<<"in B constructor"<<endl;
        throw -1;
    }
    ~B()
    {
        cout<<"in B destructor"<<endl;
        delete[]  resource;
    }
};

int main()
{
    // B *pb = nullptr;
    try
    {
        B b;
        // pb = new B;
    }
    catch(int)
    {
        cout<<"catched"<<endl;
    }

    // delete pb;
}

// in A constructor
// in C constructor
// in B constructor
// in C destructor
// in A destructor
// catched
