#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;
 
template<class Iter>
void merge_sort(Iter first, Iter last)
{
    if (last - first > 1)
    {
        Iter middle = first + (last - first) / 2;
        merge_sort(first, middle);
        merge_sort(middle, last);
        std::inplace_merge(first, middle, last);
    }
}
 
int main()
{
    std::vector<int> v{8, 2, -2, 0, 11, 11, 1, 7, 3};
    merge_sort(v.begin(), v.end());
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(cout, " "));
    cout << endl;

    return 0;
}
