#include <iostream>

using namespace std;

struct Foo {
    bool flag_ = false;

    void test() {
        flag_ = true;
        // struct _OnDone {
            // ~_OnDone() { flag_ = false; }
        // } _on_done; // WRONG
        std::unique_ptr<void, std::function<void(void*)>>
                _on_done((void*)-1, [this](void*){flag_ = false;});
        cout << "test() flag=" << flag_ << endl;
    }
};

int main() {
    Foo foo;
    foo.test();
    cout << "main flag=" << foo.flag_ << endl;

    return 0;
}

