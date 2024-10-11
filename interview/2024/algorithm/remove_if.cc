#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

int main() {
    vector<int> arr{1,3,5,2,4,7,8};
    auto it = std::remove_if(arr.begin(), arr.end(), [](int val)->bool {
        return val % 2 == 0;
    });  // 返回值是删除后区间的末尾

    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(cout, " "));
    cout << endl;
    cout << std::distance(arr.begin(), it) << endl;
    std::copy(arr.begin(), it, std::ostream_iterator<int>(cout, " "));
    cout << endl;

    return 0;
}
/*
 * 1 3 5 7 4 7 8
 * 4
 * 1 3 5 7
 */
