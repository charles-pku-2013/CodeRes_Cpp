#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

void process( string &str )
{
    const char *nullstr = "NULL";

    string::iterator it = str.end();
    if( it == str.begin() )
        return;

    --it;
    while( it != str.begin() ) {
        if( it > str.begin() && *it == '\t' && *(it-1) == '\t' )
            it = str.insert( it, nullstr, nullstr + 4 );
        --it;
    } // while
}


int main()
{
    string line;

    while( getline(cin, line) ) {
        process( line );
        cout << line << endl;
    }

    return 0;
}

/*
 * int main( int argc, char **argv )
 * {
 *     test();
 *     string line;
 *     char *pLine = NULL, *pField = NULL;
 *     vector<char*> arr;
 *     uint32_t lineCount = 0;
 * 
 *     while( getline(cin, line) )
 *     {
 *         ++lineCount;
 *         arr.clear();
 *         pLine = const_cast<char*>(line.c_str());
 *         for( pField = strtok(pLine, "\t"); pField; pField = strtok(NULL, "\t") ) 
 *             arr.push_back(pField);
 *         
 *         if( arr.size() != 13 )
 *         {
 *             cout << lineCount << " line is invalid, n_field = " << arr.size() << endl;
 *         } // if
 *     } // while
 * 
 *     return 0;
 * }
 */


