/* 时间测量 time */
using namespace std::chrono; // see §35.2
auto t0 = high_resolution_clock::now();
do_work();
auto t1 = high_resolution_clock::now();
cout << duration_cast<milliseconds>(t1−t0).count() << "msec\n";

using namespace std::chrono; // see §35.2
auto t0 = high_resolution_clock::now();
this_thread::sleep_for(milliseconds{20});
auto t1 = high_resolution_clock::now();
cout << duration_cast<nanoseconds>(t1−t0).count() << " nanoseconds passed\n";


/* 条件变量 */
void consumer()
{
    while(true) {
        unique_lock<mutex> lck{mmutex}; // acquire mmutex
        while (mcond.wait(lck)) /* do nothing */; // release lck and wait;
        // re-acquire lck upon wakeup
        auto m = mqueue.front(); // get the message
        mqueue.pop();
        lck.unlock(); //release lck
        // ... process m ...
    }
}

void producer()
{
    while(true) {
        Message m;
        // ... fill the message ...
        unique_lock<mutex> lck {mmutex}; // protect operations
        mqueue.push(m);
        mcond.notify_one(); // notify
    } // release lock (at end of scope)
}

















