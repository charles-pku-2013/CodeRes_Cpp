#include <iostream>
#include <sstream>

using namespace std;

template< typename StreamType >
bool bad_stream( const StreamType &stream )
{ return (stream.fail() || stream.bad()); }

template< typename StreamType >
void print_stream_state( const StreamType &stream )
{
    cout << "good: " << stream.good() << endl;
    cout << "eof: " << stream.eof() << endl;
    cout << "fail: " << stream.fail() << endl;
    cout << "bad: " << stream.bad() << endl;
    cout << bitset<4>(stream.rdstate()).to_string() << endl;
}

void test() {
    // read fields
    std::string s("name 123");
    std::istringstream iss(s);

    std::string name;
    int val1 = 0, val2 = 0;

    iss >> name >> val1 >> val2;
    cout << "name:" << name << endl;
    cout << "val1:" << val1 << endl;
    cout << "val2:" << val2 << endl;
    print_stream_state(iss);
}

int main()
{
    test();

    // NOTE!!! 用 fail() 判断读取是否成功
    /*
     * 1
     * good: 0
     * eof: 1
     * fail: 1
     * bad: 0
     */
    // string s;
    // string s("           ");

    /*
     * 0
     * good: 0
     * eof: 1
     * fail: 0
     * bad: 0
     */
    string s("  tg1234  ");

    int value = 0;
    stringstream str(s);
    str >> value;
    cout << "value = " << value << endl;
    cout << bad_stream(str) << endl;
    print_stream_state(str);

    return 0;
}
