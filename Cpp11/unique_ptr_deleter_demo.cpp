#include <iostream>
#include <memory>
#include <mutex>

struct Foo {
    Foo() { std::cout << "Foo construct" << std::endl; }
    ~Foo() { std::cout << "Foo destruct" << std::endl; }
    int a = 0;
};

Foo* new_foo() {
    return new Foo;
}
// never call if p is nullptr
void delete_foo(Foo *p) {
    std::cout << "delete_foo called" << std::endl;
    delete p;
}
void test1() {
    std::unique_ptr<Foo, decltype(&delete_foo)> ptr(nullptr, delete_foo);
    ptr.reset(new_foo());
    // std::unique_ptr<Foo, decltype(&delete_foo)> ptr(new_foo(), delete_foo);
    // ptr.reset();  // manully free
}

void initialize_foo(Foo *p) {
    std::cout << "initialize foo" << std::endl;
    p->a = 10;
}
void destory_foo(Foo *p) {
    std::cout << "destroy foo" << std::endl;
    p->a = 0;
}
// on stack
void test2() {
    Foo foo;
    initialize_foo(&foo);
    // std::unique_ptr<Foo, decltype(&destory_foo)> ptr(nullptr, destory_foo);
    // ptr.reset(&foo);
    std::unique_ptr<Foo, decltype(&destory_foo)> ptr(&foo, destory_foo);
}

int main() {
    // test1();
    test2();

    std:: cout << "main terminate..." << std::endl;

    return 0;
}


#if 0
std::unique_ptr<std::FILE, decltype(&std::fclose)>
            fp(std::fopen("demo.txt", "r"), &std::fclose);

std::unique_ptr<TF_Buffer, decltype(&TF_DeleteBuffer)>
        op_list_buf(TF_GetAllOpList(), &TF_DeleteBuffer);

std::unique_ptr<TF_Status, decltype(&TF_DeleteStatus)>
        status(TF_NewStatus(), &TF_DeleteStatus);

std::unique_ptr<D, std::function<void(D*)>> p(new D, [](D* ptr)
        {
            std::cout << "destroying from a custom deleter...\n";
            delete ptr;
        });  // p owns D
    p->bar();
#endif
