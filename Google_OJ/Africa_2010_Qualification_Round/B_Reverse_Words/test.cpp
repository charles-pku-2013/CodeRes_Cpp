#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>

#define BLANK " \t\f\r\v\n"

using namespace std;

void process( char *str, int c )
{
    vector<char*> vec;
    for( char *p = strtok(str, BLANK); p; p = strtok(NULL, BLANK) )
        vec.push_back(p);
    reverse(vec.begin(), vec.end());
    printf( "Case #%d: ", c );
    for( vector<char*>::iterator it = vec.begin(); it != vec.end()-1; ++it )
        printf( "%s ", *it );
    printf( "%s\n", vec.back() );
}


int main()
{
    int nCases;
    string line;
    
    scanf( "%d\n", &nCases );
    for( int c = 1; c <= nCases; ++c ) {
        getline(cin, line);
        process((char*)(line.c_str()), c);
    }
    
    return 0;
}