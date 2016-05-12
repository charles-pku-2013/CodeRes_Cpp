#include <cstring>
#include <iostream>

#define CONST_STR "Hello"

int main()
{
    using namespace std;

    const char *cstr = "world" CONST_STR;

    cout << cstr << endl;

    return 0;
}

