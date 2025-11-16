#include <iostream>
#include <cstring> // For strcpy, strlen
#include <algorithm> // For std::swap

// Base Class
class Base {
protected:
    int base_data;
    char* base_name;

public:
    Base(int data, const char* name) : base_data(data) {
        base_name = new char[strlen(name) + 1];
        strcpy(base_name, name);
    }

    // Custom Copy Constructor
    Base(const Base& other) : base_data(other.base_data) {
        std::cout << "Base copy constructor called" << std::endl;
        base_name = new char[strlen(other.base_name) + 1];
        strcpy(base_name, other.base_name);
    }

    // Custom Copy Assignment Operator
    Base& operator=(const Base& other) {
        std::cout << "Base copy assignment operator called" << std::endl;
        if (this != &other) {
            base_data = other.base_data;
            delete[] base_name; // Free existing memory
            base_name = new char[strlen(other.base_name) + 1]; // Allocate new memory
            strcpy(base_name, other.base_name); // Copy data
        }
        return *this;
    }

    // Destructor
    virtual ~Base() {
        std::cout << "Base destructor called" << std::endl;
        delete[] base_name;
    }

    void display_base() const {
        std::cout << "Base Data: " << base_data << ", Base Name: " << base_name << std::endl;
    }
};

// Derived Class
class Derived : public Base {
private:
    float derived_data;

public:
    Derived(int b_data, const char* b_name, float d_data) : Base(b_data, b_name), derived_data(d_data) {}

    // Custom Copy Constructor
    Derived(const Derived& other) : Base(other), derived_data(other.derived_data) {
        std::cout << "Derived copy constructor called" << std::endl;
        // The Base part is handled by the Base(other) call in the initializer list
        // The derived_data member is copied directly
    }

    // Custom Copy Assignment Operator (using copy-and-swap idiom for exception safety)
    Derived& operator=(const Derived& other) {
        std::cout << "Derived copy assignment operator called" << std::endl;
        if (this != &other) {
            // Explicitly call the base class assignment operator
            Base::operator=(other);
            // Copy derived class members
            derived_data = other.derived_data;
        }
        return *this;
    }

    // Destructor is implicitly called in the correct order (derived then base)

    void display_derived() const {
        display_base();
        std::cout << "Derived Data: " << derived_data << std::endl;
    }
};

int main() {
    Derived d1(10, "Original", 20.5f);
    std::cout << "d1 contents:" << std::endl;
    d1.display_derived();

    std::cout << "\nCalling copy constructor (d2 initialized from d1):" << std::endl;
    Derived d2 = d1; // Calls copy constructor
    std::cout << "d2 contents:" << std::endl;
    d2.display_derived();

    Derived d3(1, "Temp", 1.0f);
    std::cout << "\nInitial d3 contents:" << std::endl;
    d3.display_derived();

    std::cout << "\nCalling copy assignment operator (d3 = d1):" << std::endl;
    d3 = d1; // Calls copy assignment operator
    std::cout << "d3 contents:" << std::endl;
    d3.display_derived();

    std::cout << "\nEnd of main, destructors will be called:" << std::endl;

    return 0;
}
