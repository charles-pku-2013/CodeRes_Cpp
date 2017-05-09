#include <iostream>
#include <sstream>
#include <memory>
#include <stdexcept>
#include <boost/format.hpp>

#define THROW_RUNTIME_ERROR(x) \
    do { \
        std::stringstream __err_stream; \
        __err_stream << x; \
        __err_stream.flush(); \
        throw std::runtime_error( __err_stream.str() ); \
    } while (0)

#define THROW_RUNTIME_ERROR_IF(cond, args) \
    do { \
        if (cond) THROW_RUNTIME_ERROR(args); \
    } while (0)


using namespace std;

struct Bar {
    Bar() { cout << "Bar constructor" << endl; }
    ~Bar() { cout << "Bar destructor" << endl; }

    double zz;
};

struct Foo {
    Foo(bool flag) : pBar(new Bar)
    {
        cout << "Foo constructor flag = " << flag << endl;
        THROW_RUNTIME_ERROR_IF(flag, "Exception in constructor!");
    }

    ~Foo() { cout << "Foo destructor" << endl; }

    std::shared_ptr<Bar>    pBar;
    double x, y, z;
};


int main()
{
    Foo *ptr = NULL;

    try {
        ptr = new Foo(true);   
    } catch (const std::exception &ex) {
        cerr << ex.what() << endl;
    } // try

    // NOTE!!! 当构造函数抛出异常失败后，ptr不会被赋值，无内存泄漏
    cout << boost::format("ptr = %lx") % (uint64_t)ptr << endl;
    if (ptr) delete ptr;

    return 0;
}

/*
 * Bar constructor
 * Foo constructor flag = 1
 * Bar destructor   // NOTE!!! 构造函数抛出异常时，会自动析构已经初始化的自动变量，
 *                  // 不包括那些要在析构函数中显示删除的，如delete close
 * Exception in constructor!
 * ptr = 0
 *
 * NOTE!!! 如果在类析构函数中需要释放资源，那么在构造函数中不能获取资源后抛出异常
 * 可采用空构造函数+init()方式
 */
