#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

template<typename Container>
std::string arr2str(const Container& c) {
    std::stringstream oss;
    std::copy(c.begin(), c.end(), std::ostream_iterator<typename Container::value_type>(oss, " "));
    return oss.str();
}

int main() {
    std::vector<int> arr{1, 2, 3};
    cout << arr2str(arr) << endl;

    return 0;
}
