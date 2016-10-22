/*
 * c++ -o /tmp/test test.cpp -lboost_thread -lboost_system -lglog -pthread -std=c++11 -g
 * GLOG_logtostderr=1 /tmp/test
 * NOTE!!! upgrade_lock is exclusive
 */
#include <boost/thread.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include <boost/date_time.hpp>
#include <ctime>
#include <iostream>
#include <deque>
#include <functional>
#include <thread>
#include <glog/logging.h>

#define THIS_THREAD_ID        boost::this_thread::get_id()
#define SLEEP_SECONDS(x)      boost::this_thread::sleep_for(boost::chrono::seconds(x))
#define SLEEP_MILLISECONDS(x) boost::this_thread::sleep_for(boost::chrono::milliseconds(x))

struct Foo : public boost::upgrade_lockable_adapter<boost::shared_mutex> {
    void read()
    {
        this->lock_upgrade();
        LOG(INFO) << "Thread " << THIS_THREAD_ID << " reading...";
        SLEEP_MILLISECONDS(3000);
        LOG(INFO) << "Thread " << THIS_THREAD_ID << " read done!";
        this->unlock_upgrade();
    }
};

int main(int argc, char **argv)
{
    google::InitGoogleLogging(argv[0]);

    Foo foo;

    std::thread rdThr1(std::bind(&Foo::read, &foo));
    std::thread rdThr2(std::bind(&Foo::read, &foo));
    std::thread rdThr3(std::bind(&Foo::read, &foo));

    rdThr1.join();
    rdThr2.join();
    rdThr3.join();

    return 0;
}

