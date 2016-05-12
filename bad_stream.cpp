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

int main()
{
    string s("           ");
    cout << s.length() << endl;

    string value;
    stringstream str(s);
    str >> value;
    cout << bad_stream(str) << endl;
    print_stream_state(str);

    return 0;
}
