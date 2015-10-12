#ifndef _WRAP_UNIX_H
#define _WRAP_UNIX_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include "error.h"

#define FILE_MODE   (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#ifndef PATH_MAX            /* should be in <limits.h> */
#define PATH_MAX    1024    /* max # of characters in a pathname */
#endif


//!! must link with -lrt
inline int
Shm_open(const char *pathname, int oflag, mode_t mode)
{
    int     fd;

    if ( (fd = shm_open(pathname, oflag, mode)) == -1)
        err_sys("shm_open error for %s", pathname);
    return(fd);
}

inline void
Shm_unlink(const char *pathname)
{
    if (shm_unlink(pathname) == -1)
        err_sys("shm_unlink error");
}

inline void *
Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
    void    *ptr;

    if ( (ptr = mmap(addr, len, prot, flags, fd, offset)) == MAP_FAILED)
        err_sys("mmap error");
    return(ptr);
}

inline void
Munmap(void *addr, size_t len)
{
    if (munmap(addr, len) == -1)
        err_sys("munmap error");
}

inline void
Ftruncate(int fd, off_t length)
{
    if (ftruncate(fd, length) == -1)
        err_sys("ftruncate error");
}

inline void
Fstat(int fd, struct stat *ptr)
{
    if (fstat(fd, ptr) == -1)
        err_sys("fstat error");
}

inline void
Close(int fd)
{
    if (close(fd) == -1)
        err_sys("close error");
}

#endif // _WRAP_UNIX_H
