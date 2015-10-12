#ifndef _WRAPPTHREAD_H
#define _WRAPPTHREAD_H

/*
 * pthreads wrapper functions.
 */

#include    <signal.h>
#include    <pthread.h>
#include    <errno.h>
#include    "error.h"

inline void
Pthread_attr_init(pthread_attr_t *attr)
{
    int     n;

    if ( (n = pthread_attr_init(attr)) == 0)
        return;
    errno = n;
    err_sys("pthread_attr_init error");
}

inline void
Pthread_attr_destroy(pthread_attr_t *attr)
{
    int     n;

    if ( (n = pthread_attr_destroy(attr)) == 0)
        return;
    errno = n;
    err_sys("pthread_attr_destroy error");
}

inline void
Pthread_attr_setdetachstate(pthread_attr_t *attr, int detach)
{
    int     n;

    if ( (n = pthread_attr_setdetachstate(attr, detach)) == 0)
        return;
    errno = n;
    err_sys("pthread_attr_setdetachstate error");
}

inline void
Pthread_attr_setscope(pthread_attr_t *attr, int scope)
{
    int     n;

    if ( (n = pthread_attr_setscope(attr, scope)) == 0)
        return;
    errno = n;
    err_sys("pthread_attr_setscope error");
}

inline void
Pthread_create(pthread_t *tid, const pthread_attr_t *attr,
               void * (*func)(void *), void *arg)
{
    int     n;

    if ( (n = pthread_create(tid, attr, func, arg)) == 0)
        return;
    errno = n;
    err_sys("pthread_create error");
}

inline void
Pthread_join(pthread_t tid, void **status)
{
    int     n;

    if ( (n = pthread_join(tid, status)) == 0)
        return;
    errno = n;
    err_sys("pthread_join error");
}

inline void
Pthread_detach(pthread_t tid)
{
    int     n;

    if ( (n = pthread_detach(tid)) == 0)
        return;
    errno = n;
    err_sys("pthread_detach error");
}

inline void
Pthread_kill(pthread_t tid, int signo)
{
    int     n;

    if ( (n = pthread_kill(tid, signo)) == 0)
        return;
    errno = n;
    err_sys("pthread_kill error");
}

inline void
Pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
    int     n;

    if ( (n = pthread_mutexattr_init(attr)) == 0)
        return;
    errno = n;
    err_sys("pthread_mutexattr_init error");
}

inline void
Pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
    int     n;

    if ( (n = pthread_mutexattr_destroy(attr)) == 0)
        return;
    errno = n;
    err_sys("pthread_mutexattr_destroy error");
}

inline void
Pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int flag)
{
    int     n;

    if ( (n = pthread_mutexattr_setpshared(attr, flag)) == 0)
        return;
    errno = n;
    err_sys("pthread_mutexattr_setpshared error");
}

inline void
Pthread_mutex_init(pthread_mutex_t *mptr, pthread_mutexattr_t *attr)
{
    int     n;

    if ( (n = pthread_mutex_init(mptr, attr)) == 0)
        return;
    errno = n;
    err_sys("pthread_mutex_init error");
}

inline void
Pthread_mutex_destroy(pthread_mutex_t *mptr)
{
    int     n;

    if ( (n = pthread_mutex_destroy(mptr)) == 0)
        return;
    errno = n;
    err_sys("pthread_mutex_destroy error");
}

/* include Pthread_mutex_lock */
inline void
Pthread_mutex_lock(pthread_mutex_t *mptr)
{
    int     n;

    if ( (n = pthread_mutex_lock(mptr)) == 0)
        return;
    errno = n;
    err_sys("pthread_mutex_lock error");
}
/* end Pthread_mutex_lock */

inline void
Pthread_mutex_unlock(pthread_mutex_t *mptr)
{
    int     n;

    if ( (n = pthread_mutex_unlock(mptr)) == 0)
        return;
    errno = n;
    err_sys("pthread_mutex_unlock error");
}

inline void
Pthread_condattr_init(pthread_condattr_t *attr)
{
    int     n;

    if ( (n = pthread_condattr_init(attr)) == 0)
        return;
    errno = n;
    err_sys("pthread_condattr_init error");
}

inline void
Pthread_condattr_destroy(pthread_condattr_t *attr)
{
    int     n;

    if ( (n = pthread_condattr_destroy(attr)) == 0)
        return;
    errno = n;
    err_sys("pthread_condattr_destroy error");
}

inline void
Pthread_condattr_setpshared(pthread_condattr_t *attr, int flag)
{
    int     n;

    if ( (n = pthread_condattr_setpshared(attr, flag)) == 0)
        return;
    errno = n;
    err_sys("pthread_condattr_setpshared error");
}

inline void
Pthread_cond_broadcast(pthread_cond_t *cptr)
{
    int     n;

    if ( (n = pthread_cond_broadcast(cptr)) == 0)
        return;
    errno = n;
    err_sys("pthread_cond_broadcast error");
}

inline void
Pthread_cond_signal(pthread_cond_t *cptr)
{
    int     n;

    if ( (n = pthread_cond_signal(cptr)) == 0)
        return;
    errno = n;
    err_sys("pthread_cond_signal error");
}

inline void
Pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr)
{
    int     n;

    if ( (n = pthread_cond_wait(cptr, mptr)) == 0)
        return;
    errno = n;
    err_sys("pthread_cond_wait error");
}

inline void
Pthread_cond_timedwait(pthread_cond_t *cptr, pthread_mutex_t *mptr,
                       const struct timespec *tsptr)
{
    int     n;

    if ( (n = pthread_cond_timedwait(cptr, mptr, tsptr)) == 0)
        return;
    errno = n;
    err_sys("pthread_cond_timedwait error");
}

inline void
Pthread_once(pthread_once_t *ptr, void (*func)(void))
{
    int     n;

    if ( (n = pthread_once(ptr, func)) == 0)
        return;
    errno = n;
    err_sys("pthread_once error");
}

inline void
Pthread_key_create(pthread_key_t *key, void (*func)(void *))
{
    int     n;

    if ( (n = pthread_key_create(key, func)) == 0)
        return;
    errno = n;
    err_sys("pthread_key_create error");
}

inline void
Pthread_setcancelstate(int state, int *oldstate)
{
    int     n;

    if ( (n = pthread_setcancelstate(state, oldstate)) == 0)
        return;
    errno = n;
    err_sys("pthread_setcancelstate error");
}

inline void
Pthread_setspecific(pthread_key_t key, const void *value)
{
    int     n;

    if ( (n = pthread_setspecific(key, value)) == 0)
        return;
    errno = n;
    err_sys("pthread_setspecific error");
}


#endif // _WRAPPTHREAD_H
