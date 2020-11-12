#include <iostream>
#include <vector>

using namespace std;

void print(const vector<int> &arr)
{
    /*
     * NOTE!!! const auto it, it is const vector<int>::const_iterator
     */
    for (/* const */ auto it = arr.begin(); it != arr.end(); ++it)
        cout << *it << " ";
    cout << endl;
}

int main()
{
    vector<int> arr = {1,2,3,4,5};

    print(arr);

    return 0;
}

