#ifndef _SHARED_BUFFER_H
#define _SHARED_BUFFER_H

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp> 
#include <boost/interprocess/sync/interprocess_condition.hpp> 
#include <boost/interprocess/sync/scoped_lock.hpp> 
#include <memory>
#include <functional>
#include <cstring>
// #include "LOG.h"

#define SHARED_BUFSIZE              3
#define INIT_FRAME_SIZE             (256*1024)
#define ENCODER_SERVER_BUFSIZE      (5*1024*1024)       // 5MB

typedef boost::interprocess::managed_shared_memory::segment_manager         segment_manager_t;
typedef boost::interprocess::allocator<void, segment_manager_t>             void_allocator;
typedef boost::interprocess::allocator<char, segment_manager_t>             char_allocator;

//!! container 构造实例时必须传入具体的allocator

struct BytesArray : boost::interprocess::vector<char, char_allocator> {
    typedef boost::interprocess::vector<char, char_allocator>               BaseType;

    BytesArray( const void_allocator &void_alloc ) : BaseType( void_alloc ) {}
    BytesArray( size_type n, const void_allocator &void_alloc ) : BaseType( void_alloc ) 
    { reserve(n); }

    char* ptr() { return &(*this)[0]; }
    const char* ptr() const { return &(*this)[0]; }

    void append( const void *src, size_type n )
    {
        size_type cursize = this->size();
        this->resize( cursize + n );
        char *dst = ptr() + cursize;
        ::memcpy( dst, src, n );
    }
};

typedef boost::interprocess::allocator<BytesArray, segment_manager_t>       BytesArrayAllocator;

class SharedBuffer {
public:
    SharedBuffer( const void_allocator &void_alloc, std::size_t _QueSize, std::size_t _ArrSize = 0 ) 
                    : queue(void_alloc), front(0), rear(0), queSize(_QueSize + 1)
                    , readBuf_(void_alloc), writeBuf_(void_alloc)
    {
        queue.resize( queSize, BytesArray(void_alloc) );
        if( _ArrSize ) {
            for( std::size_t i = 0; i < queSize; ++i )
                queue[i].reserve( _ArrSize );
            readBuf_.reserve( _ArrSize );
            writeBuf_.reserve( _ArrSize );
        } // if 

        // for dbg
        printf("SharedBuffer constructor queSize = %lu arrSize = %lu @%lx\n", queSize, _ArrSize, (long)this);
    }

    // for dbg
    ~SharedBuffer()
    { printf("SharedBuffer destructor @%lx\n", (long)this); }

    // bool full()
    // {
        // boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lk(lock);
        // return _full();
    // }

    // bool empty()
    // {
        // boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lk(lock);
        // return _empty();
    // }

    BytesArray& writeBuf() { return writeBuf_; }
    const BytesArray& readBuf() const { return readBuf_; }

    void push()
    {
        // DBG("push @%lx", (long)this);
        boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lk(lock);

        while( _full() )
            condWr.wait(lk);
            
        queue[rear].swap( writeBuf_ );
        rear = (rear + 1) % queSize;

        lk.unlock();
        condRd.notify_one();
    }

    void pop()
    {
        // DBG("pop @%lx", (long)this);
        boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> lk(lock);

        while( _empty() )
            condRd.wait(lk);
            
        queue[front].swap( readBuf_ );
        front = (front + 1) % queSize;

        lk.unlock();
        condWr.notify_one();
    }

protected:
    bool _full()  { return ((rear + 1) % queSize == front); }
    bool _empty() { return (front == rear); }

protected:
    std::size_t                                                             front, rear;
    const std::size_t                                                       queSize;
    boost::interprocess::interprocess_mutex                                 lock;
    boost::interprocess::interprocess_condition                             condRd;
    boost::interprocess::interprocess_condition                             condWr;
    boost::interprocess::vector<BytesArray, BytesArrayAllocator>            queue;
    BytesArray                                                  readBuf_, writeBuf_;
};


typedef std::shared_ptr<SharedBuffer>       SharedBufferPtr;



#endif

