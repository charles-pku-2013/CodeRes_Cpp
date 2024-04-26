#include <iostream>
#include <memory>
#include <thread>

using namespace std;

struct Foo {
    void greet() {
        std::string s = "hello";
        _greet(s);
        float v = 3.14;
        _greet(v);
    }

    template<typename T>
    void _greet(const T& val) {}

    template<>
    void _greet<std::string>(const std::string &s) {
        cout << s << endl;
    }
    template<>
    void _greet<float>(const float& v) {
        cout << v << endl;
    }
};

int main() {
    Foo foo;
    foo.greet();

    return 0;
}
