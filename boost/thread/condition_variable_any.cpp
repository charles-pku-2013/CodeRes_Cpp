/*
 * c++ -o /tmp/test condition_variable_any.cpp -lboost_thread-mt -lboost_system -std=c++11 -pthread -g
 */
#include <boost/thread.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/format.hpp>
#include <thread>
#include <iostream>
#include <cstdio>
#include <unistd.h>

using namespace std;

struct Foo : public boost::shared_lockable_adapter<boost::shared_mutex> {
    void wait() {
        boost::shared_lock<Foo> lck(*this);
        cond.wait(lck, [&, this]{ return ready; });
    }

    void notify() {
        cond.notify_all();
    }

    bool ready = false;
    boost::condition_variable_any cond;
};

void work_routine(Foo &foo, int id) {
    foo.wait();
    printf("Thread %d ready!\n", id);
}


int main() {
    Foo foo;

    std::thread thr1(work_routine, std::ref(foo), 1);
    std::thread thr2(work_routine, std::ref(foo), 2);

    cout << "main run..." << endl;
    ::sleep(3);
    foo.ready = true;
    foo.notify();

    thr1.join();
    thr2.join();

    return 0;
}
