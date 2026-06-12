#include <iostream>
#include <vector>

using namespace std;

void print(int** arr, int row, int col) {
    for (auto i = 0; i < row; ++i) {
        for (auto j = 0; j < col; ++j) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    std::vector<int> arr{1,2,3,4,5,6};

    auto* p = arr.data();
    print(&p, 2, 3);

    return 0;
}
