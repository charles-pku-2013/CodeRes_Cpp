#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

int main()
{
    vector<int> arr{1,2,3,4,5};

    do {
        std::copy(arr.begin(), arr.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
    } while (std::next_permutation(arr.begin(), arr.end()));

    return 0;
}

