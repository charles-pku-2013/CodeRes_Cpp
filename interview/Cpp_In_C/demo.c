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

// 格式: parent_vtable
//       virtual func ptr list
//       虚表只有这一个，没有 Derived_vtable
typedef struct Base_vtable {
    const void *parentVtable;
    void (*greet)(struct Base*);       // 虚函数指针
    void (*destruct)(struct Base*);    // 虚函数指针
} Base_vtable;  // 生成了两个对象 vtableBase and vtableDerived

typedef struct Base {
    const Base_vtable *vtable;  // vtable指针放到Base类成员里，或Derived的Base部分里
    int x;
} Base;

// Base virtual functions
void Base_greet(Base *self)
{ printf("Base::greet() x = %d\n", self->x); }

void destructBase(Base *self)
{ printf("Base destructor\n"); }

// init Base vtable object （之前Base类里的vtable指针指向这里）给虚表指针赋值
const Base_vtable vtableBase = {
    NULL,               // parent vtable
    Base_greet,         // void (*greet)(struct Base*)
    destructBase        // void (*destruct)(struct Base*)
};

// Base non-virtual functions
void constructBase(Base *self, int _X)
{
    self->vtable = &vtableBase;
    self->x = _X;
}

// 一般(no-virtual)函数的实现
void Base_printX(const Base *self)
{ printf("x = %d\n", self->x); }


typedef struct Derived {
    Base inherited;  // Derived 里的 Base 部分 Derived的vtable在其Base部分里
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

// init derived vtable object
const Base_vtable vtableDerived = {
    &vtableBase,                        // parent vtable
    // Derived override func ptrs
    (void(*)(Base*)) Derived_greet,     // void (*greet)(struct Base*) 🔴🔴 类型转换是要有的
    (void(*)(Base*)) destructDerived    // void (*destruct)(struct Base*)
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

