/*
 * c++ -o /tmp/test test.cpp -lboost_thread-mt -lboost_system -pthread -std=c++11 -g
 */
#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <boost/thread.hpp>

#define THIS_THREAD_ID        std::this_thread::get_id()
#define SLEEP_SECONDS(x)      std::this_thread::sleep_for(std::chrono::seconds(x))
#define SLEEP_MILLISECONDS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))

using namespace std;

struct Foo {
    void read() {
        // boost::upgrade_lock<boost::shared_mutex> lck(mutex_);
        boost::shared_lock<boost::shared_mutex> lck(mutex_);
        cout << "Thread " << THIS_THREAD_ID << " reading..." << endl;;
        SLEEP_SECONDS(1);
        cout << "Thread " << THIS_THREAD_ID << " done" << endl;
    }

    boost::shared_mutex mutex_;
};

int main() {
    using ThreadPtr = std::shared_ptr<std::thread>;

    Foo foo;
    vector<ThreadPtr> arr;
    for (int i = 0; i < 5; ++i)
        arr.emplace_back(std::make_shared<std::thread>(&Foo::read, &foo));

    for (int i = 0; i < arr.size(); ++i)
        arr[i]->join();

    cout << "Test done!" << endl;

    return 0;
}

