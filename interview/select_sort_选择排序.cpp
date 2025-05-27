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

void SelectSort(int *a, int n) {
    if (!a || n <= 1) { return; }

    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < n; ++j) {
            if (a[j] < a[minIdx]) {
                minIdx = j;
            }
        }  // for j

        if (minIdx != i) {
            std::swap(a[i], a[minIdx]);
        }
    } // for i
}

template <typename Iter>
void SelectSort(Iter beg, Iter end)
{
    if (beg >= end || beg + 1 == end) return;

    for (; beg != end - 1; ++beg) {
        Iter minIt = beg;
        for (Iter j = beg + 1; j != end; ++j) {
            if (*j < *minIt)
                minIt = j;
        } // for j
        std::iter_swap(beg, minIt);
    } // for
}

void test1()
{
    vector<int> a{ 1, 3, 5, 7, 9 };
}

void test2()
{
    vector<int> a{ 5, 7, 4, 2, 8, 6, 1, 9, 0, 3 };
    SelectSort(a.begin(), a.end());
    copy(a.begin(), a.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
}


int main(int argc, char **argv)
{
    try {
        //test1();
        test2();
    } catch (const std::exception &ex) {
        cerr << "Exception caught by main: " << ex.what() << endl;
        return -1;
    }

    return 0;
}



