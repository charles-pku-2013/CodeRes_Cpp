#ifndef _LOG_H
#define _LOG_H

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <sys/time.h>


//!! __VA_ARGS__ 只是原样展开...
#define DBG(...) do { \
            struct timeval __dbg_timeval;   \
            gettimeofday( &__dbg_timeval, NULL );   \
            fprintf( stdout, "[%010llu,%06llu] ", (unsigned long long)(__dbg_timeval.tv_sec), \
                            (unsigned long long)(__dbg_timeval.tv_usec) );      \
            fprintf( stdout,  "%s:%d in %s(): ", __FILE__, __LINE__, __func__ ); \
            fprintf( stdout, __VA_ARGS__ ); \
            fprintf( stdout, "\n" ); \
        } while(0)

//!! args可以是包含空格和其他任意符号如<<的字符串
#define DBG_STREAM(args) do { \
            struct timeval __dbg_timeval;   \
            gettimeofday( &__dbg_timeval, NULL );   \
            std::stringstream __dbg_stream_stringstream; \
            __dbg_stream_stringstream << "[" << std::setfill('0') << std::setw(10) \
                            << (unsigned long long)(__dbg_timeval.tv_sec) \
                            << "," << std::setw(6) << (unsigned long long)(__dbg_timeval.tv_usec) << "] "; \
            __dbg_stream_stringstream << __FILE__ << ":" << __LINE__ << " in " \
                            << __func__ << "(): " << args; \
            fprintf(stdout, "%s\n", __dbg_stream_stringstream.str().c_str()); \
        } while(0)

#define DBG_COND(cond, ...) do { \
            if(cond) DBG(__VA_ARGS__);  \
        } while(0)

#define DBG_STREAM_COND(cond, args) do { \
            if(cond) DBG_STREAM(args); \
        } while(0)






#endif
