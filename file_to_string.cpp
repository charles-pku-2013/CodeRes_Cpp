#include <iostream>
#include <cstdio>
#include <string>
#include <streambuf>
#include <iterator>

using namespace std;
 
int main()
{
    // 没有换行符 都挤在一块
    // istream_iterator<char> beg(cin), end;

    // This OK
    istreambuf_iterator<char> beg(cin), end;

    std::string str(beg, end);
    cout << str << endl;

    return 0;
}
