#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>

#define BLANK " \t\f\r\v\n"

using namespace std;

void rstripString( string &str )
{
    char *start = (char*)(str.c_str());
    char *p = start + str.length() - 1;
    while( p >= start && isspace(*p) )
    *p-- = 0;
}


int main()
{

    
    return 0;
}