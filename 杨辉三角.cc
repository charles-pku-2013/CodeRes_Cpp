#include <iostream>
#include <vector>
#include <iterator>

using namespace std;

void YangTriangle(int n) {
    if (n <= 0) { return; }
    using Triangle = std::vector<std::vector<int>>;
    Triangle triangle;

    auto print_triangle = [&] {
        for (auto& row : triangle) {
            std::copy(row.begin(), row.end(), std::ostream_iterator<int>(cout, " "));
            cout << endl;
        }
    };

    // triangle.resize(n);
    triangle.emplace_back(1, 1);
    if (n >=2 ) {
        triangle.emplace_back(2, 1);
    }

    for (int i = 2; i < n; ++i) {
        auto& last = triangle[i - 1];
        triangle.emplace_back(last.size() + 1, 1);
    }

    for (int i = 2; i < triangle.size(); ++i) {
        for (int j = 1; j < triangle[i].size()-1; ++j) {
            triangle[i][j] = triangle[i-1][j-1] + triangle[i-1][j];
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
    YangTriangle(20);

    return 0;
}
