#include <iostream>
#include <memory>

using namespace std;

struct Foo {
    Foo() { cout << "Foo construct" << endl; }
    ~Foo() { cout << "Foo destruct" << endl; }
    void Greet() { cout << "Hello foo" << endl; }
};

template<typename T>
void test_func(std::unique_ptr<T> ptr) {
    cout << "Begin test" << endl;
    ptr->Greet();
    cout << "End test" << endl;
}

int main() {
#if 0
    // Test 1
    {
        std::unique_ptr<Foo> pfoo(new Foo);
        // NOTE unique_ptr 只有move constructor没有copy constructor
        // 此时函数定义部分可以不用 &&
        test_func(std::move(pfoo));  // error: call to implicitly-deleted copy constructor of 'std::unique_ptr<Foo>'

        if (pfoo) {
            cout << "pfoo is not empty" << endl;
        } else {
            cout << "pfoo is empty" << endl;
        }
    }
#endif

    // since c++14
    test_func(std::make_unique<Foo>());

    // c++ 11 ok
    // test_func(std::unique_ptr<Foo>(new Foo));

    cout << "test end..." << endl;

    return 0;
}

