#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <map>
#include <cctype>

#define BLANK " \t\f\r\v\n"

using namespace std;

typedef map<char, int> ChCount;
map<char, int> chCount;


bool process( string &line )
{
//    cout << line << endl;
    chCount.clear();
    for( string::iterator it = line.begin(); it != line.end(); ++it ) {
        if( !isalpha(*it) && !isdigit(*it) )
            return false;
        ++chCount[*it];
    } // for
    
//    for( ChCount::iterator it = chCount.begin(); it != chCount.end(); ++it )
//        cout << it->first << " " << it->second << endl;
    bool flag;
    int index = 1;
    do {
        flag = false;
        for( ChCount::iterator it = chCount.begin(); it != chCount.end(); ++it ) {
            if( it->second >= index ) {
                cout << it->first;
                flag = true;
            } // if
        } // for
        ++index;
    } while( flag );
    cout << endl;
    
    return true;
}


int main()
{
    string line;
    
    while( getline( cin, line ) ) {
//        cout << line << endl;
        bool ret = process( line );
        if( !ret )
            cout << "<invalid input string>" << endl;
    } // while
    
    return 0;
}












