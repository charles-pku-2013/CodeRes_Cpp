#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

void Yanghui_Triangle(int n) {
    using Triangle = std::vector<std::vector<int>>;

    if (n < 1) {
        return;
    }

    Triangle triangle(n + 1);

    auto print_triangle = [&] {
        for (const auto& row : triangle) {
            if (!row.empty()) {
                std::copy(row.begin(), row.end(), std::ostream_iterator<int>(cout, " "));
                cout << endl;
            }
        }
    };

    triangle[1].resize(1, 1);
    if (n >= 2) {
        triangle[2].resize(2, 1);
    }

    for (int i = 3; i <= n; ++i) {
        auto& row = triangle[i];
        auto& last_row = triangle[i - 1];
        row.resize(i, 1);
        for (int j = 1; j < row.size() - 1; ++j) {
            row[j] = last_row[j - 1] + last_row[j];
        }
    }

    print_triangle();
}

int main() {
    Yanghui_Triangle(10);

    return 0;
}

