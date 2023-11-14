#include <iostream>

struct Foo {
    // based on template specialization
    template<typename T>
    T get_value() const { return T(); }

    template<>
    int32_t get_value<int32_t>() const { return val_int_; }

    template<>
    float get_value<float>() const { return val_f_; }

    int32_t val_int_ = 5;
    float   val_f_ = 3.14;
};

int main() {
    using namespace std;

    Foo f;
    cout << f.get_value<int32_t>() << endl;
    cout << f.get_value<float>() << endl;

    return 0;
}
