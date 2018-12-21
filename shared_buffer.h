#ifndef _SHARED_BUFFER_H
#define _SHARED_BUFFER_H

#include "wrapunix.h"
#include "wrappthread.h"
#include <cstring>
#include <memory>
#include <iterator>
#include <algorithm>
#include <boost/circular_buffer.hpp>
#include "LOG.h"

//!! shm_open 路径必须以 / 开头，真正存在哪里不用管。教科书上的 px_ipc_name 是不对的
#define SHARED_NAME             "/shared_buffer.shm"
#define SHARED_BUFSIZ           20


static void* pBaseAddr = 0;         // Base addr of shared buffer

//!! 只能为一个容器对象服务，而且要一次性分配好内存reserve
template <typename T, size_t SIZE>
class FixedSizeAllocator : public std::allocator<T> {
public:
    typedef typename std::allocator<T>          BaseType;
    typedef typename BaseType::size_type        size_type;
    typedef typename BaseType::pointer          pointer;
public:
    //!! 注意 _Count 是T类型元素个数，SIZE是内存空间总大小
    pointer allocate( size_type _Count, const void * _Hint = NULL )
    {
        if( _Count > SIZE / sizeof(T) )
            throw std::bad_alloc();
        return (pointer)pBaseAddr;
    }

    void deallocate( pointer _Ptr, size_type _Count ){}

    size_type max_size() const
    { return SIZE / sizeof(T); }

    template<class OTHER>
    struct rebind {
        typedef FixedSizeAllocator<OTHER, SIZE> other;
    };
};


//!! 用共享内存传递复杂对象千万要当心，该对象一定不能有指针成员
struct SharedBufferFail : boost::circular_buffer< char, FixedSizeAllocator<char, SHARED_BUFSIZ> > {
    typedef boost::circular_buffer< char, FixedSizeAllocator<char, SHARED_BUFSIZ> >        BaseType;

    SharedBufferFail( size_t _Capacity ) : BaseType(_Capacity)
    {
        pthread_mutexattr_t mattr;
        pthread_condattr_t  cattr;

        Pthread_mutexattr_init(&mattr);
        Pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
        Pthread_mutex_init(&lock, &mattr);
        Pthread_mutexattr_destroy(&mattr);

        Pthread_condattr_init(&cattr);
        Pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
        pthread_cond_init(&readCond, &cattr);
        pthread_cond_init(&writeCond, &cattr);
        Pthread_condattr_destroy(&cattr);   /* be sure to destroy */
    }

    ~SharedBufferFail()
    {
        Pthread_mutex_destroy(&lock);
    }

    ssize_t read(void *vptr, size_t n)
    {
        char *ptr = (char*)vptr;

        Pthread_mutex_lock( &lock );

        while( this->empty() ) {
            DBG("Buffer is empty.");
            Pthread_cond_wait( &readCond, &lock );
        } // while

        n = ( n > this->size() ? this->size() : n );
        std::copy( this->begin(), this->end(), ptr );
        this->erase_begin( n );

        Pthread_mutex_unlock( &lock );
        Pthread_cond_signal( &writeCond );

        return n;
    }

    ssize_t write(void *vpBuf, size_t n)
    {
        char *ptr = (char*)vpBuf;

        Pthread_mutex_lock( &lock );

        while( this->full() ) {
            DBG("Buffer is full.");
            Pthread_cond_wait( &writeCond, &lock );
        } // while

        n = ( n > this->reserve() ? this->reserve() : n );
        std::copy( ptr, ptr + n, std::back_inserter(*this) );

        Pthread_mutex_unlock( &lock );
        Pthread_cond_signal( &readCond );

        return n;
    }

    pthread_mutex_t         lock;
    pthread_cond_t          readCond;
    pthread_cond_t          writeCond;
};


struct SharedBuffer {
    SharedBuffer( size_t _Capacity ) : capacity(_Capacity), curPos(0)
    {
        pthread_mutexattr_t mattr;
        pthread_condattr_t  cattr;

        Pthread_mutexattr_init(&mattr);
        Pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
        Pthread_mutex_init(&lock, &mattr);
        Pthread_mutexattr_destroy(&mattr);

        Pthread_condattr_init(&cattr);
        Pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
        pthread_cond_init(&readCond, &cattr);
        pthread_cond_init(&writeCond, &cattr);
        Pthread_condattr_destroy(&cattr);   /* be sure to destroy */
    }

    ~SharedBuffer()
    {
        Pthread_mutex_destroy(&lock);
    }

    bool empty() const { return (curPos == 0); }
    bool full() const { return (curPos == capacity); }

    ssize_t Read(void *vptr, size_t n)
    {
        Pthread_mutex_lock( &lock );

        while( this->empty() ) {
            DBG("Buffer is empty.");
            Pthread_cond_wait( &readCond, &lock );
        } // while

        n = ( n > curPos ? curPos : n );
        size_t nRemain = curPos - n;
        memcpy( vptr, pBaseAddr, n );
        memmove( pBaseAddr, (char*)pBaseAddr + n, nRemain );
        curPos -= n;

        Pthread_mutex_unlock( &lock );
        Pthread_cond_signal( &writeCond );

        return n;
    }

    ssize_t Write(void *vpBuf, size_t n)
    {
        Pthread_mutex_lock( &lock );

        while( this->full() ) {
            DBG("Buffer is full.");
            Pthread_cond_wait( &writeCond, &lock );
        } // while

        size_t nFree = capacity - curPos;
        n = (n > nFree ? nFree : n);
        memcpy( (char*)pBaseAddr + curPos, vpBuf, n );
        curPos += n;

        Pthread_mutex_unlock( &lock );
        Pthread_cond_signal( &readCond );

        return n;
    }

    pthread_mutex_t         lock;
    pthread_cond_t          readCond;
    pthread_cond_t          writeCond;
    const size_t            capacity;
    size_t                  curPos;
};



#endif // _SHARED_BUFFER_H
