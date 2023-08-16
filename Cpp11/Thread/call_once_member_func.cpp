#include <iostream>
#include <mutex>
#include <thread>
// #include <mutex>

class Foo {
 public:
    void Init() {
        std::call_once(once_flag_, &Foo::_DoInit, this);
    }

 private:
    void _DoInit() {
        std::cout << "Foo initing..." << std::endl;
    }

    std::once_flag once_flag_;
};


int main()
{
    Foo foo;
    std::thread t1(&Foo::Init, &foo);
    std::thread t2(&Foo::Init, &foo);
    std::thread t3(&Foo::Init, &foo);
    std::thread t4(&Foo::Init, &foo);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}


