#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>
#include <thread>
#include <memory>

/*
 * 选择排序、快速排序、希尔排序、堆排序不是稳定的排序算法。
 * 冒泡排序、插入排序、归并排序、基数排序是稳定的排序算法。
 */

using namespace std;

template <typename Iter>
void BubbleSort(Iter beg, Iter end)
{
    if (beg >= end || beg + 1 == end) return;

    std::size_t n = std::distance(beg, end);

    for (size_t j = 1; j < n; ++j) {
        for (Iter i = beg; i != end - j; ++i) {
            if (*i > *(i + 1))
                std::iter_swap(i, i + 1);
        } // for i
    } // while
}

void BubbleSort(int *a, int n) {
    if (!a || n <= 1) {
        return;
    }

    bool exchanged = true;
    for (int i = 0; exchanged && i < n - 1; ++i) {
        exchanged = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (a[j] > a[j + 1]) {
                std::swap(a[j], a[j + 1]);
                exchanged = true;
            }
        } // for j
    } // for i
}


void test1()
{
    vector<int> a{ 1, 3, 5, 7, 9 };
}

void test2()
{
    vector<int> a{ 5, 7, 4, 2, 8, 6, 1, 9, 0, 3 };
    // BubbleSort(a.begin(), a.end());
    BubbleSort(a.data(), a.size());
    copy(a.begin(), a.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
}


int main(int argc, char **argv)
{
    try {
        //test1();
        test2();
    }
    catch (const std::exception &ex) {
        cerr << "Exception caught by main: " << ex.what() << endl;
        return -1;
    }

    return 0;
}



