/*
 * c++ -o /tmp/test shared_lockable_adapter_demo.cpp -lboost_thread -lboost_system -lglog -pthread -std=c++11 -g
 * GLOG_logtostderr=1 /tmp/test
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

struct Foo : public boost::shared_lockable_adapter<boost::shared_mutex> {
    void read()
    {
        boost::shared_lock<Foo> rlock(*this);
        LOG(INFO) << "Thread " << THIS_THREAD_ID << " reading...";
        SLEEP_MILLISECONDS(3000);
        LOG(INFO) << "Thread " << THIS_THREAD_ID << " read done!";
    }

    void write()
    {
        boost::unique_lock<Foo> wLock(*this);
        LOG(INFO) << "Thread " << THIS_THREAD_ID << " writting...";
        SLEEP_MILLISECONDS(3000);
        LOG(INFO) << "Thread " << THIS_THREAD_ID << " write done!";
    }
};

int main(int argc, char **argv)
{
    google::InitGoogleLogging(argv[0]);

    Foo foo;

    std::thread rdThr1(std::bind(&Foo::read, &foo));
    std::thread rdThr2(std::bind(&Foo::read, &foo));
    std::thread rdThr3(std::bind(&Foo::read, &foo));
    std::thread wrThr1(std::bind(&Foo::write, &foo));

    rdThr1.join();
    rdThr2.join();
    rdThr3.join();
    wrThr1.join();

    return 0;
}

/*
 * 貌似解决了写者饥饿问题
I1023 00:07:45.969214  2900 shared_lockable_adapter_demo.cpp:22] Thread 7fcb19de6700 reading...
I1023 00:07:45.969230  2901 shared_lockable_adapter_demo.cpp:22] Thread 7fcb195e5700 reading...
I1023 00:07:48.970055  2901 shared_lockable_adapter_demo.cpp:24] Thread 7fcb195e5700 read done!
I1023 00:07:48.970055  2900 shared_lockable_adapter_demo.cpp:24] Thread 7fcb19de6700 read done!
I1023 00:07:48.970276  2903 shared_lockable_adapter_demo.cpp:30] Thread 7fcb185e3700 writting...
I1023 00:07:51.971706  2903 shared_lockable_adapter_demo.cpp:32] Thread 7fcb185e3700 write done!
I1023 00:07:51.972004  2902 shared_lockable_adapter_demo.cpp:22] Thread 7fcb18de4700 reading...
I1023 00:07:54.973295  2902 shared_lockable_adapter_demo.cpp:24] Thread 7fcb18de4700 read done!
*/
