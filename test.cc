#include <boost/interprocess/sync/interprocess_recursive_mutex.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <iostream>
#include <string>
#include <thread> // For simulating multiple acquisitions within a process

int main(int argc, char* argv[]) {
    using namespace boost::interprocess;
    using namespace std;

    // Unique name for the shared memory segment and mutex
    const char* shm_name = "MySharedMemory";
    const char* mutex_name = "MyRecursiveMutex";

    // Remove previous shared memory if it exists
    // shared_memory_object::remove(shm_name);

    if (argc > 1) {
        shared_memory_object::remove(shm_name);
        return 1;
    }

    // Create a managed shared memory segment
    managed_shared_memory segment(open_or_create, shm_name, 65536);
    // managed_shared_memory segment(open_only, shm_name);
    // managed_shared_memory segment;
    // try {
        // segment = managed_shared_memory(open_or_create, shm_name, 65536); // 64KB
    // } catch (const std::exception& ex) {
        // cerr << ex.what() << endl;
        // shared_memory_object::remove(shm_name);
        // return 1;
    // }
    // if (argc == 1) {
        // segment = managed_shared_memory(create_only, shm_name, 65536); // 64KB
    // } else {
        // segment = managed_shared_memory(open_only, shm_name);
    // }

    // Construct the interprocess_recursive_mutex in shared memory
    interprocess_recursive_mutex* mutex = segment.construct<interprocess_recursive_mutex>(mutex_name)();

    std::cout << "Acquiring lock (first time)" << std::endl;
    mutex->lock();
    std::cout << "Lock acquired (first time)" << std::endl;

    cin.get();
    std::cout << "Acquiring lock (second time - recursive)" << std::endl;
    mutex->lock();
    std::cout << "Lock acquired (second time)" << std::endl;

    std::cout << "Lock acquired multiple times. Press enter to end." << std::endl;

    cin.get();
    mutex->unlock();
    mutex->unlock();

    std::cout << "Locks released. Cleaning up shared memory." << std::endl;
    // shared_memory_object::remove(shm_name);

    return 0;
}
