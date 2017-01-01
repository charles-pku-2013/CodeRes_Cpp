#ifndef _SHM_BUFFER_H_
#define _SHM_BUFFER_H_

#include <pthread.h>

template <std::size_t SIZE>
struct ShmBuffer {
    // 基于共享内存的进程共享buffer，mutex condvar用于进程间同步
    ShmBuffer() : clientReady(false), serverReady(false)
    {
        // 初始化锁和条件变量，并设置为可进程间共享
        // 应该判断每个函数返回值是否成功，这里省略了，仅为演示
        pthread_mutexattr_t mattr;
        pthread_condattr_t  cattr;

        pthread_mutexattr_init(&mattr);
        pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&mtxClient, &mattr);
        pthread_mutex_init(&mtxServer, &mattr);
        pthread_mutexattr_destroy(&mattr);

        pthread_condattr_init(&cattr);
        pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
        pthread_cond_init(&condClient, &cattr);
        pthread_cond_init(&condServer, &cattr);
        pthread_condattr_destroy(&cattr);
    }

    // mutex 和 condvar 用于同步server和client进程
    bool                clientReady, serverReady;
    pthread_mutex_t     mtxClient, mtxServer;
    pthread_cond_t      condClient, condServer;
    char                buf[SIZE];
};


#endif

