#include <iostream>
#include <iterator>
#include <memory>
#include <thread>
#include <algorithm>

using namespace std;

template<typename Container>
void print_container(const Container& c) {
    using T = typename Container::value_type;
    std::copy(c.begin(), c.end(), std::ostream_iterator<T>(cout, " "));
    cout << endl;
}

template<typename Iter>
void print_range(Iter beg, Iter end) {
    using T = typename std::iterator_traits<Iter>::value_type;
    std::copy(beg, end, std::ostream_iterator<T>(cout, " "));
    cout << endl;
}

int main() {
    // std::vector<int> arr{1,2,3,4,5};  // end
    // std::vector<int> arr{5,4,3,2,1};  // 4
    std::vector<int> arr{5,1,2,3,4};  // 1
    print_container(arr);
    print_range(arr.begin(), arr.end());
    return 0;

    // NOTE 返回的it是有序数组的end, past the last one
    auto it = std::is_sorted_until(arr.begin(), arr.end());
    if (it == arr.end()) {
        cout << "END" << endl;
    } else {
        cout << *it << endl;
    }

    return 0;
}

