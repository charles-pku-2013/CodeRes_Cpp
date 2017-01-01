#include <iostream>
#include <sstream>
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


struct Foo {
    Foo(bool flag)
    {
        THROW_RUNTIME_ERROR_IF(!flag, "Exception in constructor!");
    }

    double x, y, z;
};


int main()
{
    using namespace std;

    Foo *ptr = NULL;

    try {
        ptr = new Foo(true);   
    } catch (const std::exception &ex) {
        cerr << ex.what() << endl;
    } // try

    // NOTE!!! 当构造函数抛出异常失败后，ptr不会被赋值，无内存泄漏
    cout << boost::format("ptr = %lx") % (uint64_t)ptr << endl;

    return 0;
}

