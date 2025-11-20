/*
 * In C++ inheritance, constructors are not automatically inherited by derived classes. 
 * When a derived class object is created, the base class constructor must be called to 
 * initialize the base class's members. If the base class has a parameterized constructor, 
 * the derived class constructor must explicitly call it and pass the necessary arguments.
 */
#include <iostream>

class Base {
public:
    int baseValue;

    Base(int value) : baseValue(value) {
        std::cout << "Base class parameterized constructor called with value: " << baseValue << std::endl;
    }
};

class Derived : public Base {
public:
    int derivedValue;

    Derived(int baseArg, int derivedArg) : Base(baseArg), derivedValue(derivedArg) {
        std::cout << "Derived class parameterized constructor called with derived value: " << derivedValue << std::endl;
    }

    // 万能模板
    template <typename... Args>
    Derived(Args&&... args)
        : Base(std::forward<Args>(args)...) {}
};

int main() {
    Derived d(10, 20);
    std::cout << "Accessing baseValue from derived object: " << d.baseValue << std::endl;
    std::cout << "Accessing derivedValue from derived object: " << d.derivedValue << std::endl;
    return 0;
}
