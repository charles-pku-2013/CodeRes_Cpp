#include <iostream>
#include <memory>
using namespace std;
class B;

class A {
    shared_ptr<B> sP1;  // use weak_ptr instead to avoid CD

 public:
    A() {
        cout << "A()" << endl;
    }

    ~A() {
        cout << "~A()" << endl;
    }

    void setShared(shared_ptr<B>& p) {
        sP1 = p;
    }

    void report() const {
        cout << sP1.use_count() << endl;
    }
};

class B {
    shared_ptr<A> sP1;

 public:
    B() {
        cout << "B()" << endl;
    }

    ~B() {
        cout << "~B()" << endl;
    }

    void setShared(shared_ptr<A>& p) {
        sP1 = p;
    }

    void report() const {
        cout << sP1.use_count() << endl;
    }
};

int main() {
    shared_ptr<A> aPtr(new A);
    shared_ptr<B> bPtr(new B);

    aPtr->setShared(bPtr);
    bPtr->setShared(aPtr);

    aPtr->report();  // 2
    bPtr->report();  // 2

    return 0;
}
