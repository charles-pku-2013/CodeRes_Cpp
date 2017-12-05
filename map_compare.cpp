#include <iostream>
#include <map>
#include <string>

using namespace std;

int main()
{
    map<string, int> m1 = {{"a", 1}, {"b", 2}, {"c", 3}};
    map<string, int> m2 = {{"a", 1}, {"b", 3}, {"c", 3}};

    cout << (m1 == m2) << endl;
    cout << (m1 < m2) << endl;

    return 0;
}

