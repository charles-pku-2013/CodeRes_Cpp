#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <numeric>
#include <vector>

using namespace std;

// 注意和accumulate区别，前者只返回最后计算结果
int main() {
    std::vector<int> v(10, 2); // v = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2}

    std::cout << "The first " << v.size() << " even numbers are: ";
    // write the result to the cout stream
    std::partial_sum(v.cbegin(), v.cend(),
                     std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';  // 2 4 6 8 10 12 14 16 18 20

    // write the result back to the vector v
    std::partial_sum(v.cbegin(), v.cend(),
                     v.begin(), std::multiplies<int>());  // 2 4 8 16 32 64 128 256 512 1024

    std::cout << "The first " << v.size() << " powers of 2 are: ";
    std::ostringstream ss;
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(ss, " "));
    std::cout << ss.str() << std::endl;

    // 结果第一个元素不变，后面每一个是和前一个的计算结果
    // 10的阶乘
    std::vector<int> a{1,2,3,4,5,6,7,8,9,10};
    std::partial_sum(a.begin(), a.end(), std::ostream_iterator<int>(cout, " "), std::multiplies<int>());
    cout << endl;  // 1 2 6 24 120 720 5040 40320 362880 3628800

    return 0;
}
