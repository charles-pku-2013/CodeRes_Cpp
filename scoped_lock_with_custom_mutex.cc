
#include <boost/interprocess/sync/named_recursive_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <iostream>

using namespace boost::interprocess;

struct MyMutex {
    void lock() {
        std::cout << "lock" << std::endl;
    }

    void unlock() {
        std::cout << "unlock" << std::endl;
    }
};

int main() {
    MyMutex mtx;
    scoped_lock<MyMutex> lock1(mtx);

    return 0;
}

