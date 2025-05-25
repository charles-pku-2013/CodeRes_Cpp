/*
https://en.cppreference.com/w/cpp/language/rule_of_three
Rule of three
If a class requires a user-defined destructor, a user-defined copy constructor, or a user-defined
copy assignment operator, it almost certainly requires all three. Because C++ copies and
copy-assigns objects of user-defined types in various situations (passing/returning by value,
manipulating a container, etc), these special member functions will be called, if accessible, and if
they are not user-defined, they are implicitly-defined by the compiler. The implicitly-defined
special member functions should not be used if the class manages a resource whose handle is an
object of non-class type (raw pointer, POSIX file descriptor, etc), whose destructor does nothing
and copy constructor/assignment operator performs a "shallow copy" (copies the value of the handle,
without duplicating the underlying resource).
 */
#include <cstddef>
#include <cstring>
#include <iostream>
#include <utility>

class rule_of_three {
    char* cstring;  // raw pointer used as a handle to a
                    // dynamically-allocated memory block

 public:
    explicit rule_of_three(const char* s = "") : cstring(nullptr) {
        if (s) {
            cstring = new char[std::strlen(s) + 1];  // allocate
            std::strcpy(cstring, s);                 // populate
        }
    }

    ~rule_of_three()  // I. destructor
    {
        delete[] cstring;  // deallocate
    }

    rule_of_three(const rule_of_three& other)  // II. copy constructor
        : rule_of_three(other.cstring) {}

    rule_of_three& operator=(const rule_of_three& other)  // III. copy assignment
    {
        // implemented through copy-and-swap for brevity
        // note that this prevents potential storage reuse
        rule_of_three temp(other);
        std::swap(cstring, temp.cstring);
        return *this;
    }

    const char* c_str() const  // accessor
    {
        return cstring;
    }
};

int main() {
    rule_of_three o1{"abc"};
    std::cout << o1.c_str() << ' ';
    auto o2{o1};  // II. uses copy constructor
    std::cout << o2.c_str() << ' ';
    rule_of_three o3("def");
    std::cout << o3.c_str() << ' ';
    o3 = o2;  // III. uses copy assignment
    std::cout << o3.c_str() << '\n';
}  // I. all destructors are called here
