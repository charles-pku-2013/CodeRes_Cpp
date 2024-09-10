#include <algorithm>
#include <iostream>
#include <vector>
#include "absl/strings/str_join.h"

int main() {
    using namespace std;

    std::vector<int> v{3, 1, 4, 1, 5, 9};

    std::make_heap(v.begin(), v.end());
    cout << "After make_heap: " << absl::StrJoin(v.begin(), v.end(), ",") << endl;

    // int a[] = {6, 7, 8};
    // v.insert(v.end(), a, a + std::size(a));
    // NOTE push_heap 一次只能新插入一个元素
    v.push_back(8);
    cout << "After insert: " << absl::StrJoin(v.begin(), v.end(), ",") << endl;

    // NOTE push_heap 的元素在之前的容器中, 先往容器末尾加元素，再用push_heap重构堆
    std::push_heap(v.begin(), v.end());
    cout << "After push_heap: " << absl::StrJoin(v.begin(), v.end(), ",") << endl;

    // pop_heap 把第一个元素移动到末尾，重新调整堆
    std::pop_heap(v.begin(), v.end());
    cout << "After pop_heap: " << absl::StrJoin(v.begin(), v.end(), ",") << endl;

    cout << std::is_heap(v.begin(), v.end() - 1) << endl;
    cout << std::is_heap(v.begin(), v.end()) << endl;

    return 0;
}

#if 0
After make_heap: 9,5,4,1,1,3
After insert: 9,5,4,1,1,3,8
After push_heap: 9,5,8,1,1,3,4
After pop_heap: 8,5,4,1,1,3,9
1
0
#endif
