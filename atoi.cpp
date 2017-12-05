#include <iostream>
#include <cstdlib>

using namespace std;

// NOTE!!! 转换失败返回0

int main()
{
    string s;
    // string s = " 0 "; // 0
    // string s = " 12a ";     // 12
    // string s = " a12 "; // 0
    cout << atoi(s.c_str()) << endl;

    return 0;
}

