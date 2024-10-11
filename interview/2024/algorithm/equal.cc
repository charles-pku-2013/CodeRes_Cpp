#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    std::string s("aba");

    cout << std::equal(s.begin(), s.end(), s.rbegin()) << endl;

    return 0;
}

