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
void func_called_by_thread( const std::shared_ptr<Foo> &sp )
{
    if( sp )
        cout << "sp count is: " << sp.use_count() << endl;
}

static
void thread_routine( std::weak_ptr<Foo> wp )
{
    for( ; ; ) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        //!! shared_ptr 可以隐式转换成weak_ptr，但反过来不行
        func_called_by_thread( std::shared_ptr< Foo >(wp) ); // throw bad_weak_ptr if fail.
        // if( auto sp = wp.lock() ) {
            // cout << "sp from weak_ptr use_count is: " << sp.use_count() << endl;
            // sp->greet();
        // } else {
            // cout << "Object has been destroyed!" << endl;
            // return;
        // } // if
    } // for

    return;
}
 
int main()
{
    try {
        std::shared_ptr<Foo> pFoo = std::make_shared<Foo>();
        cout << "After make_shared, pFoo use_count is: " << pFoo.use_count() << endl;

        /*
         * 下面这行用法是错误的。
         * 要知道thread对象如同 function object 一样，
         * 存储了传入参数的拷贝pFoo, 这样使之增加了一个引用。
         * 同样的问题也存在于bind，用的时候要格外注意!!!
         */
        // std::thread t1( thread_routine, pFoo );
        //!! This is the right way to use.
        std::thread t1( thread_routine, std::weak_ptr<Foo>(pFoo) );
        cout << "After creating thread, pFoo use_count is: " << pFoo.use_count() << endl;

        std::this_thread::sleep_for(std::chrono::seconds(5));
        cout << "Before reset pFoo use count is: " << pFoo.use_count() << endl;
        pFoo.reset();

        if( t1.joinable() )
            t1.join();

    } catch ( const std::exception &ex ) {
        cerr << ex.what() << endl;
        exit(-1);
    }
    
    return 0;
}













