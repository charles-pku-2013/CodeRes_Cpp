#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    string s("1,2,3 4,5,6, 7 8,9");
    char *pStr = const_cast<char*>(s.c_str());
    char *saveEnd1 = NULL, *saveEnd2 = NULL;

    for( char *p = strtok_r(pStr, " ", &saveEnd1); p; p = strtok_r(NULL, " ", &saveEnd1) ) {
        cout << p << endl;
        for( char *q = strtok_r(p, ",", &saveEnd2); q; q = strtok_r(NULL, ",", &saveEnd2) )
            cout << q << " ";
        cout << endl;
    }

    return 0;
}

