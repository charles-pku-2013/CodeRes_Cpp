#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// Item 3
template<typename Container, typename Index>
decltype(auto)
authAndAccess(Container&& c, Index i)
{
    using value_type = typename std::decay_t<Container>::value_type;
    auto sz = i + 1;
    if (c.size() < sz) {
        c.resize(sz, value_type());
    }
    return std::forward<Container>(c)[i];
}

int main() {
    std::vector<int> arr{1, 2, 3, 4, 5};
    authAndAccess(arr, 3) = 10;  // 如果返回值是auto而不是decltype(auto)则会编译出错
    cout << authAndAccess(arr, 3) << endl;

    return 0;
}

