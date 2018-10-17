/*
 * c++ -o /tmp/test move_demo.cpp -std=c++11 -g
 */
#include <iostream>
#include <vector>
#include <boost/format.hpp>

using namespace std;

struct Foo {
    Foo(const std::vector<int> &_Data) : data_(_Data) {}
    Foo(std::vector<int> &&_Data) noexcept : data_(std::move(_Data)) {}
    // Foo(std::vector<int> &&_Data) : data_(_Data) {} // WRONG

    void greet() const
    { cout << boost::format("Foo data addr: %lx") % (void*)&data_[0] << endl; }

    std::vector<int> data_;
};

int main()
{
    vector<int> a{1,2,3,4};
    cout << boost::format("a addr: %lx") % (void*)&a[0] << endl;

    Foo f1(a);
    f1.greet();
    Foo f2(std::move(a));
    f2.greet();

    // vector<int> b = a;
    // vector<int> b = std::move(a);
    // cout << boost::format("%lx") % (void*)&b[0] << endl;

    return 0;
}

