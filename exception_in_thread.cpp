#include <exception>
#include <iostream>
#include <thread>

using namespace std;

void thr_func()
{
    int k = 2 / 5;
    if (!k)
        throw runtime_error("k is zero");
}

int main()
try {
    std::thread t1(thr_func);
    t1.join();

} catch (const std::exception &ex) {
    cerr << ex.what() << endl;
}

// terminating with uncaught exception of type std::runtime_error: k is zero
