#include <cstdio>
#include <cstring>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;


void string_strtok( const string &s, string &result )
{
    static const char *SPACES = " \t\f\r\v\n";
    vector<string> strArr;
    string::size_type pStart = 0, pEnd = 0;

    while( (pStart = s.find_first_not_of(SPACES, pEnd)) != string::npos ) {
        pEnd = s.find_first_of(SPACES, pStart);
        if( pEnd == string::npos )
            pEnd = s.length();      //!! ATTENTION
        strArr.push_back( string(s.begin()+pStart, s.begin()+pEnd) );
    } // while

    copy( strArr.begin(), strArr.end(), ostream_iterator<string>(cout, "\n") );
}

void string_tokenize( string &line )
{
    do {
        last = line.find_first_of('\t', first);
        it_first = line.begin() + first;
        it_last = (last == string::npos ? line.end() : line.begin() + last);
        cout << string( it_first, it_last ) << endl;
        first = line.find_first_not_of( '\t', last );
    } while( first != string::npos );
}


int main()
{
    string s = "I am a student";
    string result;

    string_strtok( s, result );

	return 0;
}


