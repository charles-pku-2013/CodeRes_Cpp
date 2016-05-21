#include <iostream>
#include <sstream>
#include <string>
#include <exception>

#define THROW_RUNTIME_ERROR(x) \
    do { \
        std::stringstream __err_stream; \
        __err_stream << x; \
        __err_stream.flush(); \
        throw std::runtime_error( __err_stream.str() ); \
    } while (0)

#define THROW_ERROR(type, x) \
    do { \
        std::stringstream __err_stream; \
        __err_stream << x; \
        __err_stream.flush(); \
        throw type( __err_stream.str() ); \
    } while (0)


using namespace std;

void func( int n )
{
    if (n < 0)
        THROW_ERROR(std::runtime_error, "Invalid n value!");
        // THROW_ERROR(std::runtime_error, "Invalid value n = " << n);
        // THROW_RUNTIME_ERROR( "Invalid value n = " << n );
}


int main()
{
    try {
        func(-1);

    } catch (const std::exception &ex) {
        cerr << ex.what() << endl;
    }

    return 0;
}
