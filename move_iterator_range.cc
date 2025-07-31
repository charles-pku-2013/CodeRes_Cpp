#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

struct Foo {
    Foo() = default;

    Foo(Foo&& rhs) noexcept {
        cout << "Foo move constructor" << endl;
    }
};

int main(int argc, char** argv) {
    // test1
    {
        std::vector<Foo> arr1(10);
        std::vector<Foo> arr2;
        // std::move(arr1.begin(), arr1.end(), std::back_inserter(arr2));
        std::copy(std::make_move_iterator(arr1.begin()), std::make_move_iterator(arr1.end()),
                  std::back_inserter(arr2));
    }

    // test2
    {
        auto print_len = [](const auto& arr) {
            for (auto& item : arr) {
                cout << item.length() << " ";
            }
            cout << endl;
        };
        std::vector<std::string> arr1(10, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        std::vector<std::string> arr2;
        // std::move(arr1.begin(), arr1.end(), std::back_inserter(arr2));
        std::copy(std::make_move_iterator(arr1.begin()), std::make_move_iterator(arr1.end()),
                  std::back_inserter(arr2));
        print_len(arr1);
        print_len(arr2);
    }

    return 0;
}

#if 0
Foo move constructor
Foo move constructor
Foo move constructor
Foo move constructor
Foo move constructor
Foo move constructor
Foo move constructor
Foo move constructor
Foo move constructor
Foo move constructor
0 0 0 0 0 0 0 0 0 0
37 37 37 37 37 37 37 37 37 37
#endif
