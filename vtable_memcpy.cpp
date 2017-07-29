#include <iostream>
#include <cstdlib>

using namespace std;

struct Base {
    virtual void greet()
    { cout << "Base greet()" << endl; }

    virtual ~Base() = default;
};

struct D1 : Base {
    void greet() override
    { cout << "D1 greet()" << endl; }
};

struct D2 : Base {
    void greet() override
    { cout << "D2 greet()" << endl; }
};


int main(int argc, char **argv)
try {
    Base *p1 = new D1;
    Base *p2 = new D2;

    p1->greet();
    p2->greet();

    ::memcpy((void*)p1, (void*)p2, sizeof(D1));

    cout << "After memcpy:" << endl;
    p1->greet();
    p2->greet();

    return 0;

} catch (const std::exception &ex) {
    std::cerr << "Exception caught by main: " << ex.what() << std::endl;
    return -1;
}



