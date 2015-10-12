#include <iostream>

using namespace std;


struct E {
    const char* message;
    E(const char* arg) : message(arg) { }
};


void my_terminate()
{
    cout << "Call to my_terminate" << endl;
}


struct A {
    A() { cout << "In constructor of A" << endl; }
    ~A() {
        cout << "In destructor of A" << endl;
        throw E("Exception thrown in ~A()");
    }
};


struct B {
    B() { cout << "In constructor of B" << endl; }
    ~B() { cout << "In destructor of B" << endl; }
};


int main()
{
    set_terminate(my_terminate);
    
    try {
        cout << "In try block" << endl;
        A a;
        B b;
        throw("Exception thrown in try block of main()");
    } catch (const char* e) {       //!! 未能捕捉到任何异常
        cout << "Exception: " << e << endl;
    } catch (...) {
        cout << "Some exception caught in main()" << endl;
    }
    cout << "Resume execution of main()" << endl;
    
    return 0;
}


/*
 In try block
 In constructor of A
 In constructor of B
 In destructor of B
 In destructor of A
 Call to my_terminate
 libc++abi.dylib: terminate_handler unexpectedly returned
 Abort trap: 6
 */

















