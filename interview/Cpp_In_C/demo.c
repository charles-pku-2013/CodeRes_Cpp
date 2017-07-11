#include <stdio.h>

#if 0
class Base {
public:
    Base(int _X) : x(_X) {}

    void printX() const
    { cout << "x = " << x << endl; }

    virtual void greet()
    { cout << "Base::greet() x = " << x << endl; }

    virtual ~Base() { cout << "Base destructor" << endl; }

protected:
    int x;
};

class Derived : public Base {
public:
    Derived(int _X, int _Y) : Base(_X), y(_Y) {}

    void printY() const
    { cout << "y = " << y << endl; }

    virtual void greet()
    { cout << "Derived::greet() x = " << x << ", y = " << y << endl; }
private:
    int y;
};
#endif


struct Base;
struct Derived;

typedef struct Base_vtable {
    const void *parentVtable;
    void (*greet)(struct Base*);
    void (*destruct)(struct Base*);
} Base_vtable;

typedef struct Base {
    const Base_vtable *vtable;
    int x;
} Base;

// Base virtual functions
void Base_greet(Base *self)
{ printf("Base::greet() x = %d\n", self->x); }

void destructBase(Base *self)
{ printf("Base destructor\n"); }

// init Base vtable
const Base_vtable vtableBase = {NULL, Base_greet, destructBase};

// Base non-virtual functions
void constructBase(Base *self, int _X)
{
    self->vtable = &vtableBase;
    self->x = _X;
}

void Base_printX(const Base *self)
{ printf("x = %d\n", self->x); }


typedef struct Derived {
    Base inherited;
    int y;
} Derived;

// Derived virtual functions
void Derived_greet(Derived *self)
{ printf("Derived::greet() x = %d, y = %d\n", self->inherited.x, self->y); }

void destructDerived(Derived *self)
{ 
    destructBase(&(self->inherited));
    printf("Derived destructor\n"); 
}

// init derived vtable
const Base_vtable vtableDerived = {
    &vtableBase, 
    (void(*)(Base*)) Derived_greet,     // 🔴🔴 类型转换是要有的
    (void(*)(Base*)) destructDerived
};

// Derived non-virtual functions
void constructDerived(Derived *self, int _X, int _Y)
{
    constructBase(&(self->inherited), _X);
    self->inherited.vtable = &vtableDerived;
    self->y = _Y;
}


// Show how virtual function works
void call_greet(Base *p)
{ p->vtable->greet(p); }    // 🔴🔴 call in this way


int main()
{
    /* printf("%lu\n", sizeof(Base)); */
    /* printf("%lu\n", sizeof(Derived)); */
    Derived d;
    constructDerived(&d, 3, 5);
    call_greet((Base*)&d);

    return 0;
}

