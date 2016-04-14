#include <iostream>
#include <vector>
#include <cstring>
#include <cstdio>
#include <memory>

using namespace std;

struct B;

/*
 * 他们的析构函数不会被调用
 */
struct A {
    ~A()
    { cout << "A destructor." << endl; }

    long val;
    shared_ptr<B>   m_b;
    // weak_ptr<B>   m_b;
};
 
struct B {
    ~B()
    { cout << "B destructor." << endl; }

    long val;
    shared_ptr<A>   m_a;
    // weak_ptr<A>   m_a;
};
 
int main()
{
    shared_ptr<A>   a(new A);  //new出来的A的引用计数此时为1
    shared_ptr<B>   b(new B);  //new出来的B的引用计数此时为1
    a->m_b = b; //B的引用计数增加为2
    b->m_a = a; //A的引用计数增加为2
 
    /*
     * b先出作用域，B的引用计数减少为1，不为0，所以堆上的B空间没有被释放，
     * 且B持有的A也没有机会被析构，A的引用计数也完全没减少
     * a后出作用域，同理A的引用计数减少为1，不为0，所以堆上A的空间也没有被释放
     *
     * 循环引用不只是两方的情况，只要引用链成环都会出现问题。
     */

    /* 一样的 */
    // a.reset();
    // b.reset();
    // cout << "After calling reset." << endl;

    return 0; 
}


// valgrind output for circular depend.
/*
 * ==4897== HEAP SUMMARY:
 * ==4897==     in use at exit: 96 bytes in 4 blocks
 * ==4897==   total heap usage: 4 allocs, 0 frees, 96 bytes allocated
 * ==4897== 
 * ==4897== 96 (24 direct, 72 indirect) bytes in 1 blocks are definitely lost in loss record 4 of 4
 * ==4897==    at 0x4C2B0E0: operator new(unsigned long) (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
 * ==4897==    by 0x400D42: main (test.cpp:32)
 * ==4897== 
 * ==4897== LEAK SUMMARY:
 * ==4897==    definitely lost: 24 bytes in 1 blocks
 * ==4897==    indirectly lost: 72 bytes in 3 blocks
 * ==4897==      possibly lost: 0 bytes in 0 blocks
 * ==4897==    still reachable: 0 bytes in 0 blocks
 * ==4897==         suppressed: 0 bytes in 0 blocks
 */

// valgrind output when switch to weak_ptr
/*
 * ==4922== HEAP SUMMARY:
 * ==4922==     in use at exit: 0 bytes in 0 blocks
 * ==4922==   total heap usage: 4 allocs, 4 frees, 96 bytes allocated
 * ==4922== 
 * ==4922== All heap blocks were freed -- no leaks are possible
 * ==4922== 
 * ==4922== For counts of detected and suppressed errors, rerun with: -v
 * ==4922== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
 */

