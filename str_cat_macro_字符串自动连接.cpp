/*
 * str_cat_macro_字符串自动连接.cpp
 */
#include <iostream>
#include <string>

// #连接
#define GREET(x) "hello "#x


using namespace std;

int main()
{
    // cout << "Hello" 
            // "world" << endl;

    // string s = GREET("Charles") " morning!";
    string s = GREET(Charles) " morning!";
    cout << s << endl;

    return 0;
}

