#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

using namespace std;

struct Base {
    int x = 0;
};

struct Derived : Base {
    int y = 0;
};

// NOTE!!! 没有裸指针到智能指针的隐式转换
// no implicit conversion from raw pointer to smart pointer
// std::shared_ptr<Derived> func()
// { return new Derived; }

std::shared_ptr<Base> func()
{ return std::make_shared<Derived>(); }

int main() {
    auto p = func();

    return 0;
}
