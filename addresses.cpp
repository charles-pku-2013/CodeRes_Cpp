#include <iostream>
#include <cstdio>

using namespace std;

int gVar;

struct Demo {
    Demo( char *_str ) : str(_str)
    { printf("Demo constructor: %lx\n", this); }
    
    ~Demo()
    { printf("Demo destructor: %lx\n", this); }
    
    char *str;
};



int main()
{
    static int sVar;
    printf("in function static variable sVar address is: %lx\n", &sVar);            // data segment
 
    printf( "global variable gVar address is: %lx\n", &gVar );      // data segment
    
    
    const Demo &t = "abc";
    
    int a = 5;
    printf("auto var a address is: %lx\n", &a);
    
    char *str = "Hello, world";
    printf( "const string str address is: %lx\n", str );        // in data segment
    
    char str1[] = "Hello, world";
    printf( "const string array str1 address is %lx\n", str1 );
    
    char *str2 = new char[10];
    printf( "string with new str2 address is %lx\n", str2 );
    
//    getchar();
    
    return 0;
}


/*
 in function static variable sVar address is: 10c986074
 global variable gVar address is: 10c986070
 Demo constructor: 7fff5327a8b0
 auto var a address is: 7fff5327a8ac
 const string str address is: 10c985eb4
 const string array str1 address is 7fff5327a8c8
 string with new str2 address is 7f9b72403a90
 Demo destructor: 7fff5327a8b0
 */











