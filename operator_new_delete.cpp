#include <iostream>
#include <memory>
#include <vector>

using namespace std;

// 可以看看内存对齐的实现。mem align
struct Foo {
    int x;
    char c;
    // double d;

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
};

int main()
{
    // Foo *p = new Foo;
    // delete p;

    /*
     * STL 容器分配内存都是用allocator，构建对象用 placement new;
     * 销毁对象先显示调用析构函数，再 allocator::deallocate();
     * 所以不会调用自定义的 operator new / delete.
     */
    vector<Foo> vf;
    vf.resize(100);

    return 0;
}
