#include <iostream>
#include <cstdio>
#include <string>
#include <streambuf>
#include <iterator>

int main()
{
    // 没有换行符 都挤在一块
    // istream_iterator<char> beg(cin), end;

    // This OK
    std::istreambuf_iterator<char> beg(std::cin), end;

    std::string str(beg, end);
    std::cout << str << std::endl;

    return 0;
}
