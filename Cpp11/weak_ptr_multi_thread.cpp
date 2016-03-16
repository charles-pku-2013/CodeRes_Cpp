#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <functional>
 
using namespace std;


struct Foo {
    Foo() { cout << "Foo constructor." << endl; }
    virtual ~Foo() { cout << "Foo destructor." << endl; }
    void greet() { cout << "Foo says Hello" << endl; }
};
 
static
void thread_routine( std::weak_ptr<Foo> wp )
{
    for( ; ; ) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if( auto sp = wp.lock() ) {
            cout << "sp from weak_ptr use_count is: " << sp.use_count() << endl;
            sp->greet();
        } else {
            cout << "Object has been destroyed!" << endl;
            return;
        } // if
    } // for

    return;
}
 
int main()
{
    std::shared_ptr<Foo> pFoo = std::make_shared<Foo>();
    cout << "After make_shared, pFoo use_count is: " << pFoo.use_count() << endl;
    std::thread t1( thread_routine, std::weak_ptr<Foo>(pFoo) );
    cout << "After creating thread, pFoo use_count is: " << pFoo.use_count() << endl;

    std::this_thread::sleep_for(std::chrono::seconds(5));
    cout << "Before reset pFoo use count is: " << pFoo.use_count() << endl;
    pFoo.reset();

    if( t1.joinable() )
        t1.join();
    
    return 0;
}













