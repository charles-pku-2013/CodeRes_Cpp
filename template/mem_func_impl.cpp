#include <iostream>
#include <string>
#include <sstream>

template<typename T>
class Foo {
 public:
    T compute(const T& a, const T& b);
};

template<typename T>
T Foo<T>::compute(const T& a, const T& b)
{ return a + b; }


int main() {
    Foo<int> foo;
    int c = foo.compute(3,4);

    return 0;
}
