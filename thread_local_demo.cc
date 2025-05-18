#include <iostream>
#include <thread>

#define THIS_THREAD_ID        std::this_thread::get_id()

using namespace std;

thread_local int thr_local_cnt = 0;

void increment() {
    ++thr_local_cnt;
    cout << "Thread " << THIS_THREAD_ID << " cnt: " << thr_local_cnt << endl;
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    cout << "Main thread cnt: " << thr_local_cnt << endl;

    return 0;
}
