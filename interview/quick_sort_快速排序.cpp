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


using namespace std;


int partition(int *a, int len, const int pivot)
{
    int j = 0;
    for (int i = 0; i < len ++i) { // 🔴🔴 i 一路找符合条件的，放到左侧
        if (a[i] <= pivot) {        // 🔴🔴 <=
            std::swap(a[i], a[j]);
            ++j;
        } // if
    } // for

    return j; // 🔴 j左侧但不包括j全部 <= pivot, j往右包括j在内全都 > pivot
}



template <typename Iter>
Iter Partition(Iter low, Iter high)
{
    typedef typename Iter::value_type   value_type;
    --high;
    // 取最后一个数为pivot
    const value_type &pivot = *high;
    // 两个游标，low j 一前一后游动 j在前，low在后
    // 当j发现应该放在前面的数字(<= pivot)，扔给low (和low交换内容)
    // low始终指向第一个 > pivot 的数字，在此之前的都 <= pivot, 被j扔过来的
    for (Iter j = low; j != high; ++j) {
        if (*j <= pivot) {
            std::iter_swap(low, j);
            ++low;
        } // if
    } // for
    // 当这个循环结束的时候, low指向第一个 > pivot 的数字，low之前的都 <= pivot
    // 当然这个区间是除去pivot的，应该将low和pivot位置内容交换
    std::iter_swap(low, high);

    return low; // 🔴 low往左包括low都 <= pivot, low往右不包括low都 > pivot
}

template <typename Iter>
void QuickSort(Iter beg, Iter end)
{
    if (beg >= end || beg + 1 == end)
        return;
    std::size_t n = std::distance(beg, end);
    if (n == 2) {
        if (*beg > *(beg + 1))
            std::iter_swap(beg, beg+1);
        return;
    } // if

    Iter midIt = Partition(beg, end);
    QuickSort(beg, midIt);
    QuickSort(midIt + 1, end);
}

void test1()
{
    vector<int> a{ 5, 4, 3, 2, 1, 10, 9, 8, 7, 6 };
    //auto it = Partition(a.begin(), a.end());
    //cout << *it << endl;
    //copy(a.begin(), a.end(), ostream_iterator<int>(cout, " "));
    //cout << endl;
    QuickSort(a.begin(), a.end());
    copy(a.begin(), a.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
}

void test2()
{
    vector<int> a{ 5, 7, 4, 2, 8, 6, 1, 9, 0, 3 };
    auto it = Partition(a.begin(), a.end());
    cout << *it << endl;
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



