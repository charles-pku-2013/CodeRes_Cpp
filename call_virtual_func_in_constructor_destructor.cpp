#include <cstdio>
//!! 强烈不建议这样做
class Base {
public:
    Base()
    {
        printf("Base constructor\n");
        print();
    }
    virtual ~Base()
    {
        printf("Base destructor\n");
        print();
    }
    virtual void print()
    { printf("Base\n"); }
};


class Derived : public Base {
public:
    Derived()
    { printf("Derived constructor\n"); }
    ~Derived()
    { printf("Derived destructor\n"); }
    void print()
    { printf("Derived\n"); }
};


int main()
{
    Base *p = new Derived;
//    p->print();
    delete p;
    
    return 0;
}

/*
 Base constructor
 Base
 Derived constructor
 Derived destructor
 Base destructor
 Base
 */














