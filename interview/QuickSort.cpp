#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <ctime>
#include <glog/logging.h>

template<typename Iter>
std::ostream& print_range(std::ostream &os, Iter beg, Iter end)
{
    std::copy(beg, end, std::ostream_iterator<typename Iter::value_type>(os, " "));
    os << std::endl;
    return os;
}

// NOTE!!! 并不能将某个数放到最终排好序的位置
template <typename Iter, typename Predicate>
Iter Partition( Iter beg, Iter end, Predicate pred )
{
    if (beg == end) return beg; // empty

    --end;
    // 只有一个元素
    if (beg == end)
        return (pred(*beg) ? beg+1 : beg);

    while (beg < end) {
        while (pred(*beg)) ++beg; // 找第一个非法数值
        while (!pred(*end)) --end;
        if (beg < end) // 再检查
            std::iter_swap(beg, end);
    } // while
    
    return beg;
}

template <typename Iter, typename Predicate>
void QuickSort(Iter beg, Iter end, Predicate pred)
{
    // empty or only one elem
    if (beg >= end || beg+1 == end)
        return;

    // only 2 items
    // NOTE!!! 仅有2个元素也要特殊处理
    if (beg + 2 == end && !pred(*beg, *(beg+1))) {
        std::iter_swap(beg, beg+1);
        return;
    } // if
    
    Iter pivotLoc = beg + std::distance(beg, end) / 2;

    // NOTE!!! placeholders
    Iter pivot = Partition(beg, end, std::bind(pred, std::placeholders::_1, *pivotLoc));

    QuickSort(beg, pivot, pred);
    QuickSort(pivot+1, end, pred); // NOTE!!! pivot+1 not pivot
}

template <typename Iter, typename Predicate>
void PartialSort(Iter beg, Iter mid, Iter end, Predicate pred)
{
    // empty or only one elem
    if (beg >= end || beg+1 == end)
        return;

    print_range(DLOG(INFO), beg, end);

    // only 2 items
    // NOTE!!! 仅有2个元素也要特殊处理
    if (beg + 2 == end && !pred(*beg, *(beg+1))) {
        std::iter_swap(beg, beg+1);
        return;
    } // if
    
    Iter pivotLoc = beg + std::distance(beg, end) / 2;
    typename Iter::value_type val = *pivotLoc;

    // NOTE!!! placeholders
    Iter pivot = Partition(beg, end, std::bind(pred, std::placeholders::_1, val));

    if (mid > beg)
        PartialSort(beg, mid, pivot, pred);
    if (mid > pivot)
        PartialSort(pivot+1, mid, end, pred); // NOTE!!! pivot+1 not pivot
}

void test_partial_sort()
{
    using namespace std;

    // vector<int> a{1,2,3,4,5,6,7,8,9,10};
    vector<int> a{5, 7, 4, 2, 8, 6, 1, 9, 0, 3};
    // std::partial_sort(a.begin(), a.begin()+4, a.end());
    PartialSort(a.begin(), a.begin()+4, a.end(), std::less<int>());
    print_range(cout, a.begin(), a.end());
}

void test_quick_sort()
{
    using namespace std;

    // vector<int> a{1,2,3,4,5,6,7,8,9,10};
    vector<int> a{5, 7, 4, 2, 8, 6, 1, 9, 0, 3};
    // vector<int> a{1,2};
    // QuickSort(a.begin(), a.end(), std::greater<int>());
    QuickSort(a.begin(), a.end(), std::less<int>());
    print_range(cout, a.begin(), a.end());
}

void test_partition()
{
    using namespace std;

    // vector<int> a{2, 1};
    // vector<int> a{1,3,5,2,4,6};
    // vector<int> a{1,2,3,4,5,6};
    vector<int> a{5,3,4,2,0,1};
    // auto it = Partition(a.begin(), a.end(), [](const int i)->bool {return i % 2 != 0;});
    auto it = Partition(a.begin(), a.end(), std::bind(std::less<int>(), std::placeholders::_1, 2));
    // auto it = std::partition(a.begin(), a.end(), std::bind(std::less<int>(), std::placeholders::_1, 2));
    cout << (it - a.begin()) << endl;
    print_range(cout, a.begin(), a.end());
}

int main(int argc, char **argv)
{
    using namespace std;

    google::InitGoogleLogging(argv[0]);

    try {
        test_partition();
        // test_quick_sort();
        // test_partial_sort();
        return 0;

    } catch (const std::exception &ex) {
        cerr << "Exception caught by main: " << ex.what() << endl;
    } // try

    return 0;
}

