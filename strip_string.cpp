#include <iostream>
#include <string>

#define SPACES " \t\f\r\v\n"


inline
void rstrip_string( std::string &s )
{
    using namespace std;

    string::size_type pos = s.find_last_not_of( SPACES );
    if (string::npos != pos)
        s.resize(pos + 1);
    else
        s.clear();
}

inline
std::string& strip_string( std::string &s )
{
    using namespace std;

    string::size_type pEnd = s.find_last_not_of( SPACES );
    if (string::npos != pEnd) {
        ++pEnd;
    } else {
        s.clear();
        return s;
    } // if

    string::size_type pStart = s.find_first_not_of( SPACES );
    s = s.substr(pStart, pEnd - pStart);

    return s;
}

int main()
{
    using namespace std;

    string s = "   abc   ";
    // rstrip_string(s);
    strip_string(s);

    /*
     * string s = "   ";
     * // string s;
     * // rstrip_string(s);
     * strip_string(s);
     */

    cout << s << endl;
    cout << s.length() << endl;

    return 0;
}
