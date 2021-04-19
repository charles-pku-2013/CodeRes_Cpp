#include <iostream>
#include <stdexcept>

using namespace std;

void f() noexcept {
    throw runtime_error("This is an ex");
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

