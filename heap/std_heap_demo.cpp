/*
 * NOTE!!! 没有adjust_heap()
 * 只能先pop_heap() 再 push_heap()
 */
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

/*
 * Inserts the element at the position last-1 into the max heap
 * defined by the range [first, last-1).
 * NOTE!!! 前提[first, last-1)已经是堆，将元素last(一般为新插入)纳入到堆。
 */
/*
 * v: 9 5 4 1 1 3
 * before push_heap: 9 5 4 1 1 3 6
 * after push_heap:  9 5 6 1 1 3 4
 */
void push_heap_demo()
{
    std::vector<int> v { 3, 1, 4, 1, 5, 9 };

    std::make_heap(v.begin(), v.end());

    std::cout << "v: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';

    v.push_back(6);
    std::cout << "before push_heap: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';

    std::push_heap(v.begin(), v.end());

    std::cout << "after push_heap: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';
}

/*
 * Swaps the value in the position first and the value in the position last-1
 * and makes the subrange [first, last-1) into a heap.
 * This has the effect of removing the first element from the heap defined by the range [first, last).
 */
void pop_heap_demo(std::vector<int> &v)
{
    while (!v.empty()) {
        std::pop_heap(v.begin(), v.end());
        auto largest = v.back();
        cout << largest << ' ';
        v.pop_back();
    } // while
    cout << endl;
}

/*
 * heap:   9 4 5 1 1 3 
 * sorted: 1 1 3 4 5 9
 */
int sort_heap_demo() {
    std::vector<int> v = {3, 1, 4, 1, 5, 9}; 
 
    std::make_heap(v.begin(), v.end());
 
    std::cout << "heap:\t";
    for (const auto &i : v) {
        std::cout << i << ' ';
    }   
 
    std::sort_heap(v.begin(), v.end());
 
    std::cout << "\nsorted:\t";
    for (const auto &i : v) {                                                   
        std::cout << i << ' ';
    }   
    std::cout << '\n';
}

int main()
{
    // push_heap_demo();
    // exit(0);

    std::vector<int> v { 3, 1, 4, 1, 5, 9 };

    std::cout << "initially, v: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';

    // 用 operator < 建立大根堆
    std::make_heap(v.begin(), v.end());

    std::cout << "after make_heap, v: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n'; // 9 5 4 1 1 3 NOTE!!! 大根堆堆顶(第一个)元素最大，但最后一个元素不一定是最小的。

    // pop_heap_demo(v);
    // exit(0);
    /*
     * Swaps the value in the position first and the value in the position last-1
     * and makes the subrange [first, last-1) into a max heap.
     * This has the effect of removing the first (largest) element from the heap
     * defined by the range [first, last).
     * NOTE!!! pop_heap() 将(已经是堆)的第一个(最大)元素和最后一个元素交换，是swap，
     * 并重新调整剩余的元素(不包括刚刚交换出去的现最后一个元素)为堆结构。
     */
    std::pop_heap(v.begin(), v.end());
    auto largest = v.back();
    // v.pop_back();
    std::cout << "largest element: " << largest << '\n';

    std::cout << "after removing the largest element, v: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';

    return 0;
}
