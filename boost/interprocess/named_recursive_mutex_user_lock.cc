#include <boost/interprocess/sync/named_recursive_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <iostream>

using namespace boost::interprocess;

int main() {
    /*
     * named_recursive_mutex::remove(mutex_name)
     * While the destructor releases the process's hold on the mutex, the named resource itself persists until remove() is called. Implementing a strategy for cleaning up named resources (e.g., by the last process to exit or a dedicated cleanup mechanism) is crucial to avoid resource leaks.
     */
    // Create or open a named recursive mutex

    // located in /dev/shm/my_named_recursive_mutex
    // can check anyone is using it by `lsof $file`
    named_recursive_mutex named_recursive_mtx(open_or_create, "my_named_recursive_mutex");

    // Acquire the lock
    scoped_lock<named_recursive_mutex> lock1(named_recursive_mtx);
    std::cout << "First lock acquired." << std::endl;

    std::cin.get();

    scoped_lock<named_recursive_mutex> lock2(named_recursive_mtx);
    std::cout << "Second lock acquired (recursive)." << std::endl;

    std::cin.get();
    return 0;
}
