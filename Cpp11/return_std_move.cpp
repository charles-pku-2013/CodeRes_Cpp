#include <iostream>
#include <vector>
#include <boost/format.hpp>

struct Foo {
    Foo() { std::cout << boost::format("Foo construct %lx") % (uint64_t)this << std::endl; }
    ~Foo() { std::cout << boost::format("Foo destruct %lx") % (uint64_t)this << std::endl; }
};

std::vector<Foo> test() {
    std::vector<Foo> ret(5);
    // ret.resize(4);
    // return ret;
    return std::move(ret);  // 没有区别，可能是编译器优化
}

int main() {
    std::vector<Foo> vec;
    vec = test();
    std::cout << vec.size() << std::endl;

    return 0;
}

