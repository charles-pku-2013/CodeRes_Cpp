#include <iostream>
#include <string>

class Base {
public:
    int base_data;
    std::string base_name;

    Base(int data, std::string name) : base_data(data), base_name(name) {}

    // Base class copy constructor (can be implicitly generated, but defined here for clarity)
    Base(const Base& other) : base_data(other.base_data), base_name(other.base_name) {
        std::cout << "Base copy constructor called." << std::endl;
    }
};

class Derived : public Base {
public:
    int derived_data;

    // Constructor for Derived with full arguments
    Derived(int base_data_val, std::string base_name_val, int derived_data_val)
        : Base(base_data_val, base_name_val), derived_data(derived_data_val) {}

    // The specific constructor you asked for:
    // Accepts a const reference to a Base object and copies only the Base members.
    // The derived_data member must be initialized separately (e.g., to a default value).
    Derived(const Base& base_obj) : Base(base_obj), derived_data(0) {
        std::cout << "Derived constructor from Base object called." << std::endl;
        // derived_data is initialized to a default value (e.g., 0)
        // You could add logic here to set derived_data if needed
    }

    void display() {
        std::cout << "Base data: " << base_data << ", Base name: " << base_name
                  << ", Derived data: " << derived_data << std::endl;
    }
};

int main() {
    Base original_base(10, "OriginalBase");

    // Create a Derived object using the specific constructor
    Derived derived_from_base(original_base);

    derived_from_base.display();

    return 0;
}
