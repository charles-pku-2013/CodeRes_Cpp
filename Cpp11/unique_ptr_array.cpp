#include <iostream>
#include <string>
#include <memory>
#include <iterator>

using namespace std;

struct Foo {
    Foo() {
        std::cout << "Foo default construct" << std::endl;
    }

    Foo(int i) : data_(i) {
        std::cout << "Foo construct data=" << data_ << std::endl;
    }

    ~Foo() {
        std::cout << "Foo destruct data=" << data_ << std::endl;
    }

    int data_ = 0;

    friend std::ostream& operator<<(std::ostream& os, const Foo& foo) {
        os << foo.data_;
        return os;
    }
};


int main() {
    // std::unique_ptr<Foo[]> ptr(new Foo[10]{[0 ... 9] = {5}});
    std::unique_ptr<Foo[]> ptr(new Foo[10]{1,2,3,4,5,0});   // only init first elem

    std::copy(ptr.get(), ptr.get() + 10, std::ostream_iterator<Foo>(cout, " "));
    cout << endl;

    return 0;
}
