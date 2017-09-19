#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>


using namespace std;

void print_reverse(int *a, size_t n)
{
    while (n--)
        cout << a[n] << ", ";
    cout << "\b\b  " << endl; // 这种方法其实不靠谱，vim显示不正常
    cout << n << endl; // Here n is npos
}


int main(int argc, char **argv)
{
    vector<int> arr{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    print_reverse(&arr[0], arr.size());

    return 0;
}


/*
 * 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
 * 18446744073709551615
 */


