#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>

using namespace std;

std::vector<int> TopRange(const std::vector<float>& v) {
    std::vector<int> result(v.size(), 1);
    std::vector<float> stk;

    for (std::size_t i = 1; i < v.size(); ++i) {
        while (!stk.empty() && stk.back() <= v[i]) {
            stk.pop_back();
        }
        if (stk.empty()) {
            result.push_back(i + 1);
        } else {
            result.push_back(i - stk.back());
        }
    } // for i

    return result;
}

int main() {
    std::vector<float> v{13.5,13.6,13.4,13.3,13.5,13.9,13.1,20.1,20.2,20.3};
    auto result = TopRange(v);
    std::copy(result.begin(), result.end(), std::ostream_iterator<int>(cout, " "));
    cout << endl;

    return 0;
}

// 时间复杂度O(n^2)
// 应该有效率更高的解决方法比如动态规划
