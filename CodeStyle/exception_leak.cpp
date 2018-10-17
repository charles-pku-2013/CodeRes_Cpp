#include <iostream>
#include <vector>
#include <boost/format.hpp>

using namespace std;

void g(int *a)
{
    // ...
    // Throw exception here!!!
    // ...
}

void f()
{
    int *a = new int[1000000];
    // ...
    g(a);   // when throw exception, codes after will not be run
    // ...
    delete [] a;
}

int main(int argc, char **argv)
try {
    f();

    return 0;

} catch (const std::exception &ex) {
    std::cerr << "Exception caught by main: " << ex.what() << std::endl;
    return -1;
} catch (...) {
    std::cerr << "Unknown exception caught by main!" << std::endl;
    return -1;
}



