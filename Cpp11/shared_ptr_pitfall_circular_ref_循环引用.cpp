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

    shared_ptr<B>   m_b;
};
 
struct B {
    ~B()
    { cout << "B destructor." << endl; }

    shared_ptr<A>   m_a;
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

    return 0; 
}
