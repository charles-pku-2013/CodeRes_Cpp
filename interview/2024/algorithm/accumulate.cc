#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
#include <string>

using namespace std;

int main() {
    vector<int> a{1,2,3,4,5,6,7,8,9,10};
    int sum = std::accumulate(a.begin(), a.end(), 0);
    cout << sum << endl;
    int result = std::accumulate(a.begin(), a.end(), 1, [](int a, int b)->int {
        return a * b;
    });
    cout << result << endl;

    return 0;
}
