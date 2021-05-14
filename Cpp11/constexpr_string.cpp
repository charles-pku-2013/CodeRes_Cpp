#include <string>
#include <iostream>
#include <cmath>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

struct Foo {
    static constexpr const char* name = "Foo";  // OK
    static constexpr const char* Name() { return "Foo"; }  // OK
    // static constexpr const char name[] = "Foo"; // WRONG link error
};


int main() {
    using namespace std;

    Foo foo;
    cout << foo.name << endl;
    cout << foo.Name() << endl;

    return 0;
}
