#include <iostream>
#include <boost/format.hpp>

using namespace std;

struct Foo {
    Foo() { cout << "Foo construct " << boost::format("%lx") % (void*)this << endl; }
    ~Foo() { cout << "Foo destruct " << boost::format("%lx") % (void*)this << endl; }
};

template<typename T>
void void_delete(void *_p) {
    T *ptr = static_cast<T*>(_p);
    if (ptr) {
        cout << "deleting " << boost::format("%lx") % (void*)ptr << endl;
        delete ptr;
    }
}

struct Container {
    using Deleter = std::function<void(void*)>;

    ~Container() {
        cout << "Container destruct" << endl;
        if (deleter)
            deleter(ptr);
    }

    Deleter deleter;
    void *ptr = nullptr;
};


int main() {
    Container c;

    c.ptr = new Foo;
    c.deleter = void_delete<Foo>;

    return 0;
}

