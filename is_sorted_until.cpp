#include <iostream>
#include <memory>
#include <thread>
#include <algorithm>

using namespace std;

int main() {
    // std::vector<int> arr{1,2,3,4,5};  // end
    // std::vector<int> arr{5,4,3,2,1};  // 4
    std::vector<int> arr{5,1,2,3,4};  // 1

    // NOTE 返回的it是有序数组的end, past the last one
    auto it = std::is_sorted_until(arr.begin(), arr.end());
    if (it == arr.end()) {
        cout << "END" << endl;
    } else {
        cout << *it << endl;
    }

    return 0;
}

