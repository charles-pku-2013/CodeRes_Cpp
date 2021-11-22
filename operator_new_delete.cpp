#include <iostream>
#include <memory>
#include <vector>

using namespace std;

void* operator new(std::size_t sz) // no inline, required by [replacement.functions]/3
{
    cout << "global new called, size = " << sz << endl;
    if (sz == 0)
        ++sz; // avoid std::malloc(0) which may return nullptr on success

    if (void *ptr = std::malloc(sz))
        return ptr;

    throw std::bad_alloc{}; // required by [new.delete.single]/3
}

void operator delete(void* ptr) noexcept
{
    cout << "global delete called" << endl;
    std::free(ptr);
}

// 可以看看内存对齐的实现。mem align
struct Foo {
    int x;
    char c;
    // double d;

    Foo() {
        cout << "Foo construct" << endl;
    }

    //!! 注意这里的size是最终的字节数，allocator中的是元素个数
    static void* operator new( size_t sz )
    {
        cout << "Foo::new called, size = " << sz << endl;
        return std::malloc( sz );
    }

    static void* operator new[]( size_t sz )
    {
        cout << "Foo::new[] called, size = " << sz << endl;
        return std::malloc( sz );
    }

    static void operator delete( void *p )
    {
        cout << "Foo::delete called" << endl;
        std::free(p);
    }

    // 数组版delete也必须要有
    static void operator delete[]( void *p )
    {
        cout << "Foo::delete[] called" << endl;
        std::free(p);
    }
};

int main()
{
    int *pi = new int(10);
    delete pi;

    int *i_arr = new int[100];
    delete [] i_arr;

    Foo *p = new Foo;
    delete p;

    Foo *arr = new Foo[10];
    delete [] arr;

    /*
     * STL 容器分配内存都是用allocator，构建对象用 placement new;
     * 销毁对象先显示调用析构函数，再 allocator::deallocate();
     * 所以不会调用自定义的 operator new / delete.
     */
    vector<Foo> vf;
    vf.resize(100);  // will call global new

    return 0;
}
