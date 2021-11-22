#include <iostream>
#include <functional>
#include "tools/thread_pool.h"

using namespace std;

class Foo : public std::function<void()> {
 public:
    Foo(int _a, int _b) : a(_a), b(_b) {}

    void operator()(void) {
        sum = a + b;
        cout << "Foo working sum=" << sum << endl;
    }

    int getSum() const { return sum; }

 private:
    int a, b, sum;
};


int main() {
    tools::ThreadPool tp(5);
    Foo foo(3, 5);
    tp.Schedule(std::move(foo));
    // tp.Schedule(std::ref(foo));
    cin.get();
    cout << foo.getSum() << endl;

    return 0;
}


