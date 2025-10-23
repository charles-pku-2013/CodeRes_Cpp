#include <iostream>
#include <vector>
#include <iterator>

using namespace std;

int main() {
    std::vector<int> arr{1,2,3,4,5};
    cout << std::distance(arr.end(), arr.begin()) << endl;

    return 0;
}
