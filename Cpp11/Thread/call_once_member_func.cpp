#include <iostream>
#include <mutex>
#include <thread>
// #include <mutex>

class Foo {
 public:
    void Init(const std::string &cfg) {
        // NOTE!!! 注意顺序，`this` first, then args
        std::call_once(once_flag_, &Foo::_DoInit, this, cfg);
    }

 private:
    void _DoInit(const std::string &cfg) {
        std::cout << "Foo initing..." << cfg << std::endl;
    }

    std::once_flag once_flag_;
};


int main()
{
    Foo foo;
    std::thread t1(&Foo::Init, &foo, "123");
    std::thread t2(&Foo::Init, &foo, "456");
    std::thread t3(&Foo::Init, &foo, "789");

    t1.join();
    t2.join();
    t3.join();

    return 0;
}


