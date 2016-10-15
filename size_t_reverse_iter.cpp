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
        cout << a[n] << " ";
    cout << endl;
}


int main(int argc, char **argv)
{
    vector<int> arr{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    print_reverse(&arr[0], arr.size());

    return 0;
}
