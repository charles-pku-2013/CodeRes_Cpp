#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

template<typename Iter, typename Cmp = std::less<typename Iter::value_type>>
void test(Iter beg, Iter end, Cmp&& cmp = Cmp()) {
    std::sort(beg, end, cmp);
}

int main() {
    vector<int> arr{5,1,6,2,4};
    test(arr.begin(), arr.end());
    for (auto& v : arr)
        cout << v << " ";
    cout << endl;

    return 0;
}

