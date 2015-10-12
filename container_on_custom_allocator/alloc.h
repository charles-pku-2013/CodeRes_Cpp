#ifndef _ALLOC_H
#define _ALLOC_H

#include <cstring>
#include <memory>

extern void* pBaseAddr;

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
        if( _Count > max_size() )
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


#endif
