#include <iostream>
#include <string>
#include <boost/format.hpp>

using namespace std;


void test_string(const string &s)
{
    cout << boost::format("%lx") % (void*)(s.data()) << endl; 
}

void test(const char *s)
{
    cout << boost::format("%lx") % (void*)s << endl; 
    test_string(s);
}

int main()
{
    test("Hello, world!");

    return 0;
}


/*
 * 两个地址不一样，说明 const string& 创建临时变量发生了内存分配
 * 0x100508258
 * 0x7fff5f70e3e9
 */
