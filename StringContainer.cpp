#include <cstdio>
#include <vector>
#include <string>
#include <iostream>


using namespace std;


int main()
{
    // test 1
//    {
//        string s;
//        printf( "%x\n", s.c_str() );
//        s = "Hello";
//        printf( "%x\n", s.c_str() );
//        
//        //!! NOTE, memory copy occured
//        vector<string> vec;
//        vec.push_back(s);
//        printf( "%x\n", vec.back().c_str() );
//    }
    
    // test 2
    {
        //!! this is the efficient way.
        vector<string> vec;
        vec.push_back(string());
        string &s = vec.back();
        getline( cin , s );
        printf( "%lx\n", s.c_str() );
        printf( "%lx\n", vec.back().c_str() );
    }
    
    return 0;
}


















