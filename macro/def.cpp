#include <iostream>

using namespace std;

#define TEST
// #define TEST 0

int main(int argc, char *argv[])
{
#ifdef TEST
    // cout << "TEST defined value: " << TEST << endl;
    cout << "TEST defined" << endl;
#else
    cout << "TEST not defined" << endl;
#endif
    
    return 0;
}

