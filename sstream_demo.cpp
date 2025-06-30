#include <iostream>
#include <sstream>
#include <string>

using namespace std;

inline
std::size_t file_size(std::istream &fin)
{
    std::size_t pos = fin.tellg();
    fin.seekg(0, std::ios_base::end);
    std::size_t sz = fin.tellg();
    fin.seekg(pos);
    return sz;
}

void test1()
{
    stringstream stream;
    stream << 100;
    stream << ' ' << 'a';
    stream << " Hello world";
    stream.flush();
    cout << stream.tellp() << endl;  // output indicator
    cout << stream.tellg() << endl;  // input indicator
    // cout << stream.str() << endl;
    // cout << file_size(stream) << endl;
    auto sz = file_size(stream);
    string out(sz, 0);
    stream.read(&out[0], sz);
    cout << out << endl;
    cout << stream.tellp() << endl;
    cout << stream.tellg() << endl;
}

void test2()
{
    // sstream 类似队列，后面写入，前面读取
    // 要么指定初始字符串，要么全程用 << >>
    stringstream stream;
    stream << 100;
    stream << ' ' << 200;
    cout << stream.str() << endl;

    int n;
    stream >> n;
    cout << "n = " << n << endl;;
    stream << ' ' << 300;
    cout << stream.str() << endl;

    stream >> n;
    cout << "n = " << n << endl;;
}

int main()
{
    // test1();
    // return 0;

    test2();
    return 0;

    string s("100 200 300");

    stringstream stream(s);
    int n;
    stream >> n;
    cout << n << endl;

    s[5] = '5';

    stream >> n;
    cout << n << endl;
    //!! NOTE!!! 输出结果 100 200，stringstream 构造时拷贝了string

    cout << stream.tellp() << endl;
    cout << stream.tellg() << endl;

    // stream.clear();
    // stream.flush();
    stream << "500 " << flush; // NOTE!!! 覆盖原来字符串
    stream << "78" << flush;
    cout << stream.tellp() << endl;
    cout << stream.tellg() << endl;

    cout << stream.str() << endl; // NOTE!!! 500 780 300

    return 0;
}
