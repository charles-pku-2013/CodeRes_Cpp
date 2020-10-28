#include <iostream>
#include <thread>
#include <memory>

class Foo {
public:
    using pointer = std::shared_ptr<Foo>;

    static pointer GetInstance() {
        std::cout << "Foo::GetInstance()" << std::endl;
        std::call_once(flag_, []{ pInst_.reset(new Foo); });
        return pInst_;
    }

    ~Foo() {
        std::cout << "Foo denstruct" << std::endl;
    }

    void Greet() const
    { std::cout << "Hello foo" << std::endl; }

private:
    static pointer pInst_;
    static std::once_flag flag_;

    Foo() {
        std::cout << "Foo construct" << std::endl;
    }
    Foo(const Foo&) = delete;
    Foo& operator=(const Foo&) = delete;
};

Foo::pointer Foo::pInst_;
std::once_flag Foo::flag_;

int main() {
    for (int i = 0; i < 10; ++i) {
        auto inst = Foo::GetInstance();
        inst->Greet();
    }

    std::cout << "Test terminating..." << std::endl;
    return 0;
}
