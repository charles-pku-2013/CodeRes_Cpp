#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

void YangTriangle(int n) {
    if (n <= 0) { return; }

    using Triangle = std::vector<std::vector<int>>;
    Triangle triangle;

    auto print_triangle = [&] {
        // NOTE use 'auto' instead of 'const Triangle::value_type'
        std::for_each(triangle.begin(), triangle.end(), [](const auto& row) {
            std::copy(row.begin(), row.end(), std::ostream_iterator<int>(cout, " "));
            cout << endl;
        });
    };

    for (int i = 0; i < n; ++i) {
        triangle.emplace_back(i + 1, 1);
    }

    for (int i = 1; i < n; ++i) {
        auto& cur = triangle[i];
        auto& last = triangle[i - 1];
        for (int j = 1; j < cur.size() - 1; ++j) {
            cur[j] = last[j - 1] + last[j];
        }
    }

    print_triangle();
}

#if 0
1
1 1
1 2 1
1 3 3 1
1 4 6 4 1
1 5 10 10 5 1
1 6 15 20 15 6 1
1 7 21 35 35 21 7 1
1 8 28 56 70 56 28 8 1
1 9 36 84 126 126 84 36 9 1
#endif


int main() {
    YangTriangle(10);

    return 0;
}
