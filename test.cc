#include <iostream>
#include <memory>

template<typename D>
struct Holder {
    template<typename F>
    explicit Holder(F&& f)
        : deleter_(std::forward<F>(f)) {}

    ~Holder() {
        deleter_();
    }

    D deleter_;
};

int main() {
    using namespace std;

    Holder holder([]{
        cout << "done" << endl;
    });


    return 0;
}
