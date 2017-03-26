#include <iostream>
#include <sstream>
#include <stdexcept>


#define THROW_RUNTIME_ERROR(args) \
    do { \
        std::ostringstream __err_stream; \
        __err_stream << args << std::flush; \
        throw std::runtime_error( __err_stream.str() ); \
    } while (0)

#define THROW_RUNTIME_ERROR_IF(cond, args) \
    do { \
        if (cond) THROW_RUNTIME_ERROR(args); \
    } while (0)

#define RET_MSG(args) \
    do { \
        std::ostringstream __err_stream; \
        __err_stream << args << std::flush; \
        std::cerr << __err_stream.str() << std::endl; \
        return; \
    } while (0)

#define RET_MSG_IF(cond, args) \
    do { \
        if (cond) RET_MSG(args); \
    } while (0)

#define RET_MSG_VAL(val, args) \
    do { \
        std::ostringstream __err_stream; \
        __err_stream << args << std::flush; \
        std::cerr << __err_stream.str() << std::endl; \
        return val; \
    } while (0)

#define RET_MSG_VAL_IF(cond, val, args) \
    do { \
        if (cond) RET_MSG_VAL(val, args); \
    } while (0)


int main(int argc, char **argv)
try {
    RET_MSG_VAL_IF(argc < 3, -1, 
            "Usage: " << argv[0] << " dataFile confFile");

    return 0;

} catch (const std::exception &ex) {
    std::cerr << "Exception caught in main: " << ex.what() << std::endl;
    return -1;
}
