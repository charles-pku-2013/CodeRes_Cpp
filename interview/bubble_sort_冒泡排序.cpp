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

void BubbleSort(int *a, std::size_t n) {
    if (!a || n <= 1) {
        return;
    }

    for (size_t j = 1; j < n; ++j) {
        for (size_t i = 0; i < n - j; ++i) {
            if (a[i] > a[i + 1]) {
                std::swap(a[i], a[i + 1]);
            }
        } // for i
    } // for j
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



