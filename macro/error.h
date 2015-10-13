#ifndef _ERROR_H
#define _ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>


#define err_ret( retval, ... ) do { \
                            fprintf( stderr, __VA_ARGS__ ); \
                            fprintf( stderr, " %s\n", strerror(errno) ); \
                            return retval; \
                        } while(0)

inline
void err_sys(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    vfprintf( stderr, fmt, ap );
    va_end(ap);
    fprintf( stderr, " %s\n", strerror(errno) );
    exit(1);
}


#endif
