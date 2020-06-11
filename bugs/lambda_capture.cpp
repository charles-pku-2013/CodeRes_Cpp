#include <iostream>
#include <vector>
#include <functional>
#include <boost/format.hpp>

using namespace std;

struct Foo {
    Foo()
    { cout << "Foo default construct " << boost::format("%lx") % (void*)this << endl; }
    Foo(const Foo &rhs)
    { cout << "Foo copy construct " << boost::format("%lx") % (void*)this << endl; }
    void greet() const
    { cout << "Foo greet " << boost::format("%lx") % (void*)this << endl; }
};


int main() {
    using Func = std::function<void(void)>;

    Foo foo;
    std::vector<Func> functors;
    /*
     * !!!NOTE!!!
     * 循环中capture下标，一定要用值copy=, 慎用[&]
     * [=] 会copy所有的局部变量，造成不必要的开销
     * Recommended 只capture用到的变量
     */
    for (int i = 0; i < 10; ++i) {
        functors.emplace_back([&foo, i]{
            cout << i << endl;
            foo.greet();
        });
    }

    for (auto& f : functors) {
        f();
    }

    return 0;
}

#if 0
# [&, i] what is expected
Foo default construct 0x7ffee2aeb790
0
Foo greet 0x7ffee2aeb790
1
Foo greet 0x7ffee2aeb790
2
Foo greet 0x7ffee2aeb790
3
Foo greet 0x7ffee2aeb790
4
Foo greet 0x7ffee2aeb790
5
Foo greet 0x7ffee2aeb790
6
Foo greet 0x7ffee2aeb790
7
Foo greet 0x7ffee2aeb790
8
Foo greet 0x7ffee2aeb790
9
Foo greet 0x7ffee2aeb790

# [&] all by ref, i is wrong!!!
Foo default construct 0x7ffee9105790
10
Foo greet 0x7ffee9105790
10
Foo greet 0x7ffee9105790
10
Foo greet 0x7ffee9105790
10
Foo greet 0x7ffee9105790
10
Foo greet 0x7ffee9105790
10
Foo greet 0x7ffee9105790
10
Foo greet 0x7ffee9105790
10
Foo greet 0x7ffee9105790
10
Foo greet 0x7ffee9105790
10
Foo greet 0x7ffee9105790

# [=] all by copying value, foo is wrong!!!
Foo default construct 0x7ffeefb3c798
Foo copy construct 0x7ffeefb3c774
Foo copy construct 0x7ffeefb3c6fc
Foo copy construct 0x7f9ec9e000bc
Foo copy construct 0x7ffeefb3c774
Foo copy construct 0x7ffeefb3c6fc
Foo copy construct 0x7f9ec9f000ec
Foo copy construct 0x7ffeefb3c774
Foo copy construct 0x7ffeefb3c6fc
Foo copy construct 0x7f9ecb00014c
Foo copy construct 0x7ffeefb3c774
Foo copy construct 0x7ffeefb3c8d4
Foo copy construct 0x7f9ec9c00b9c
Foo copy construct 0x7ffeefb3c774
Foo copy construct 0x7ffeefb3c6fc
Foo copy construct 0x7f9ec9f0000c
Foo copy construct 0x7ffeefb3c774
Foo copy construct 0x7ffeefb3c8d4
Foo copy construct 0x7f9ec9f0001c
Foo copy construct 0x7ffeefb3c774
Foo copy construct 0x7ffeefb3c8d4
Foo copy construct 0x7f9ec9f0002c
Foo copy construct 0x7ffeefb3c774
Foo copy construct 0x7ffeefb3c8d4
Foo copy construct 0x7f9ecb10008c
Foo copy construct 0x7ffeefb3c774
Foo copy construct 0x7ffeefb3c6fc
Foo copy construct 0x7f9ec9f0003c
Foo copy construct 0x7ffeefb3c774
Foo copy construct 0x7ffeefb3c8d4
Foo copy construct 0x7f9ecb10009c
0
Foo greet 0x7f9ec9e000bc
1
Foo greet 0x7f9ec9f000ec
2
Foo greet 0x7f9ecb00014c
3
Foo greet 0x7f9ec9c00b9c
4
Foo greet 0x7f9ec9f0000c
5
Foo greet 0x7f9ec9f0001c
6
Foo greet 0x7f9ec9f0002c
7
Foo greet 0x7f9ecb10008c
8
Foo greet 0x7f9ec9f0003c
9
Foo greet 0x7f9ecb10009c
#endif
