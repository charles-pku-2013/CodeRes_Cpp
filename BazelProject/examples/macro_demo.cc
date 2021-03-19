/*
bazel build --copt="-DTEST" -c opt //examples:macro_demo
 */
#include <iostream>

using namespace std;

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

