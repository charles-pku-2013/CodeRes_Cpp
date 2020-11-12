/*
 * c++ -o /tmp/test main.cpp Foo.cpp -g
 */
#include <iostream>
#include "Foo.h"

using namespace std;

/*
 * 原意是想在main开始之前初始化类的static成员，很好。
 * C/C++中不同编译单元内的静态变量初始化顺序是不确定的，参见：
 * https://stackoverflow.com/questions/19618801/when-are-global-variables-created
 * 但同一编译单元(cpp源文件)全局变量初始化顺序严格按照定义顺序。
 * 
 * The order in which global variables (that is, variables at namespace scope) 
 * is created is specified at being: 
            the order in which they appear, in a translation unit
            unspecified, from one translation unit to another
 * 
 * 函数内的静态变量第一次调用时初始化
 */
// static bool _dummy_init = Foo::init();   // NOTE!!! 应该放到定义这些静态变量的源文件中

int main()
{
    cout << "main()" << endl;
    return 0;
}

