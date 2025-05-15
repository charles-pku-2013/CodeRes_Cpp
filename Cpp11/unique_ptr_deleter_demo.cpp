#include <iostream>
#include <memory>
#include <vector>

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
    // std::unique_ptr<Foo, decltype(&delete_foo)> ptr;  // WRONG no default constructor
    std::unique_ptr<Foo, decltype(&delete_foo)> ptr(nullptr, delete_foo);
    ptr.reset(new_foo());

    std::vector<std::unique_ptr<Foo, decltype(&delete_foo)>> arr;
    arr.emplace_back(nullptr, delete_foo);
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

struct Container {
    std::unique_ptr<Foo, decltype(&delete_foo)> ptr{nullptr, delete_foo};  // OK NOTE!!! initialization list is required
};

int main() {
    using namespace std;

    auto cleanup = [](void*) {
        cout << "Cleaning up" << endl;
    };

    // std::unique_ptr<void, decltype(cleanup)> holder(nullptr, cleanup); // WRONG!!! cannot use nullptr here
    std::unique_ptr<void, decltype(cleanup)> holder((void*)1, cleanup);

    // test1();
    // test2();

    // Container c;
    // std:: cout << "main terminate..." << std::endl;

    return 0;
}


#if 0
std::unique_ptr<wasm_engine_t, decltype(&wasm_engine_delete)> engine_{nullptr, wasm_engine_delete};
engine_.reset(wasm_engine_new());

wasm_byte_vec_t wasm_data;
wasm_byte_vec_new_uninitialized(&wasm_data, wasm_str.size());
std::unique_ptr<wasm_byte_vec_t, decltype(&wasm_byte_vec_delete)>
            _wasm_data_deleter(&wasm_data, wasm_byte_vec_delete);

std::unique_ptr<std::FILE, decltype(&std::fclose)>
            fp(std::fopen("demo.txt", "r"), std::fclose);

std::unique_ptr<TF_Buffer, decltype(&TF_DeleteBuffer)>
        op_list_buf(TF_GetAllOpList(), TF_DeleteBuffer);

std::unique_ptr<TF_Status, decltype(&TF_DeleteStatus)>
        status(TF_NewStatus(), TF_DeleteStatus);

std::unique_ptr<D, std::function<void(D*)>> p(new D, [](D* ptr)
        {
            std::cout << "destroying from a custom deleter...\n";
            delete ptr;
        });  // p owns D
    p->bar();
#endif
