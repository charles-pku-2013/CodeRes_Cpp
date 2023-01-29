#include <iostream>
#include <string>
#include <memory>
#include <sstream>

template<typename T>
class Outer {
 public:
    class Inner;

    std::unique_ptr<Inner> ptr_;
};

template<typename T>
class Outer<T>::Inner {
};


int main() {
    Outer<int> out;
    out.ptr_.reset(new Outer<int>::Inner);

    return 0;
}
