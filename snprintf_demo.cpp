#include <cstdio>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main()
{
    /*
     * At most buf_size - 1 characters are written
     */
    {
        char buf[5];
        snprintf(buf, 5, "Hello world!");   // 最多写入指定的sz-1个字符
        cout << buf << endl;  // Hell
    }

    /*
     * Calling std::snprintf with zero buf_size and null pointer for buffer is useful 
     * to determine the necessary buffer size to contain the output:
     */
    {
        const char *fmt = "sqrt(2) = %f";
        int sz = std::snprintf(nullptr, 0, fmt, std::sqrt(2)); // 不输出，只计算所需长度
        std::vector<char> buf(sz + 1); // note +1 for null terminator
        std::snprintf(&buf[0], buf.size(), fmt, std::sqrt(2));
        cout << &buf[0] << endl;
    }

    return 0;
}
