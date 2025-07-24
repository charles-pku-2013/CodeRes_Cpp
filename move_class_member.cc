#include <iostream>
#include <vector>

using namespace std;

struct Foo {
    Foo(vector<int> arg) : data_(std::move(arg)) {}

    vector<int>& data() { return data_; }  // 0 after move
    // vector<int>&& data() { return std::move(data_); }  // 0 after move
    // const vector<int>& data() const { return data_; }  // 4 after move

    vector<int> data_;
};

int main(int argc, char **argv) {
    vector<int> arr{1,2,3,4};

    Foo foo(std::move(arr));
    cout << foo.data_.size() << endl;

    auto b = std::move(foo.data());
    cout << foo.data_.size() << endl;

    return 0;
}
