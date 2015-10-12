#include <cstdio>
#include <string>
#include <cstring>

using namespace std;

int main()
{
    // block
    {
        string s(50, 0);
        printf("%lu, %lu, %lu\n", s.length(), s.size(), strlen(s.c_str()));
        //!! string.length() 不是以'\0'来判断的
        sprintf( const_cast<char*>(s.c_str()), "Hello, world!" );
        printf("%lu, %lu, %lu\n", s.length(), s.size(), strlen(s.c_str()));
    }
    
    // block
    {
        //!! 正常情况 size() = length() 不包含末尾的'\0'
        string s("Hello, world!");
        printf("%lu, %lu, %lu\n", s.length(), s.size(), strlen(s.c_str()));
    }
    
    return 0;
}

/*
 50, 50, 0
 50, 50, 13
 13, 13, 13
 */















