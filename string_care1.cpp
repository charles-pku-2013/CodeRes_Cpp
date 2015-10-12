#include <cstdio>
#include <string>
#include <cstring>

using namespace std;

void func( const string &s )
{
    printf("string arg in func address is: %lx\n", s.c_str());
}

int main()
{
    char *s = "Hello, world";
    printf("The raw string address is: %lx\n", s);
    
    func( s );

    return 0;
}