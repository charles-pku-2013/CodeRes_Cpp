#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

int main() {
    using namespace std;

    std::vector<int> arr{1,2,3,4,5,6,7,8,9,10};

    std::size_t i = 0, j = 0;
    for (; i < arr.size(); ++i) {
        if (arr[i] % 2 == 1) {
            std::swap(arr[i], arr[j]);
            ++j;
        }
    }

    // std::partition(arr.begin(), arr.end(), [](int i)->bool {
        // return i % 2 == 1;
    // });

    std::copy(arr.begin(), arr.end(), std::ostream_iterator<int>(cout, " "));
    cout << endl;

    return 0;
}
