#include <iostream>
#include <thread>
#include <cstdio>

#define THIS_THREAD_ID        std::this_thread::get_id()
#define SLEEP_SECONDS(x)      std::this_thread::sleep_for(std::chrono::seconds(x))

using namespace std;

void worker() {
    while (true) {
        std::cout << "Thread " << THIS_THREAD_ID << " running..." << std::endl;
        SLEEP_SECONDS(1);
    }
}


int main() {
    std::thread t1(worker);
    pthread_t thread_id = 0;

    SLEEP_SECONDS(5);
    std::cout << "Trying to stop thread" << std::endl;
    if (t1.joinable()) {
        thread_id = t1.native_handle();
        cout << "native_handle = " << thread_id << endl;
        int status = pthread_cancel(thread_id);
        if (status) {
            std::cout << strerror(status) << std::endl;
        } else {
            t1.join();
        }
    }

    cout << "After join, native_handle = " << t1.native_handle() << endl;  // 0

    return 0;
}

