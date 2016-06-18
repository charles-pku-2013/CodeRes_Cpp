#include <iostream>
#include <sstream>

using namespace std;

#define ERR_RET(args) \
    do { \
        std::stringstream __err_stream; \
        __err_stream << args; \
        __err_stream.flush(); \
        std::cerr << __err_stream.str() << std::endl; \
        return; \
    } while (0)

#define COND_RET(cond, args) \
    do { \
        if (cond) ERR_RET(args); \
    } while (0)

#define ERR_RET_VAL(val, args) \
    do { \
        std::stringstream __err_stream; \
        __err_stream << args; \
        __err_stream.flush(); \
        std::cerr << __err_stream.str() << std::endl; \
        return val; \
    } while (0)

#define COND_RET_VAL(cond, val, args) \
    do { \
        if (cond) ERR_RET_VAL(val, args); \
    } while (0)

void func( int n )
{
    COND_RET(n < 0, "Invalid n value " << n);
    cout << n << endl;
}

int main()
{
    func(3);

    return 0;
}
