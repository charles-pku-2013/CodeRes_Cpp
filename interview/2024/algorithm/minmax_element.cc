#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

int main() {
    vector<int> a{3, 9, 1, 4, 2, 5, 9};
    auto ret = std::minmax_element(a.begin(), a.end());
    cout << *ret.first << " " << *ret.second << endl;

    return 0;
}

