/*
 * NOTE!!! macro must be provided
 * c++ -o /tmp/test try_lock_until_test.cpp -lboost_thread -lboost_chrono -lboost_system -lglog -pthread -std=c++11 -g -DBOOST_THREAD_USES_CHRONO -DBOOST_THREAD_PROVIDES_SHARED_MUTEX_UPWARDS_CONVERSIONS
 */
#include <boost/thread.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include <boost/chrono.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <functional>
#include <glog/logging.h>

#define THIS_THREAD_ID        boost::this_thread::get_id()
#define SLEEP_SECONDS(x)      boost::this_thread::sleep_for(boost::chrono::seconds(x))
#define SLEEP_MILLISECONDS(x) boost::this_thread::sleep_for(boost::chrono::milliseconds(x))

#define TIMEOUT         5

using namespace std;

struct SafeString : std::string
                  , boost::upgrade_lockable_adapter<boost::shared_mutex> {};

struct Foo {
    void process()
    {
        auto deadline = boost::chrono::steady_clock::now() + boost::chrono::seconds(TIMEOUT);
        boost::unique_lock<SafeString> lockName(name, boost::defer_lock);
        boost::unique_lock<SafeString> lockTitle(title, boost::defer_lock);

        if (lockName.try_lock_until(deadline)) {
            cout << "Thread " << THIS_THREAD_ID << " got name lock." << endl;
            SLEEP_SECONDS(3);
            if (lockTitle.try_lock_until(deadline)) {
                cout << "Thread " << THIS_THREAD_ID << " got title lock." << endl;
                cout << "Thread " << THIS_THREAD_ID << " start work..." << endl;
                SLEEP_SECONDS(4);
                cout << "Thread " << THIS_THREAD_ID << " finished work." << endl;
            } else {
                cout << "Thread " << THIS_THREAD_ID << " failed to get title lock." << endl;
            } // if
        } else {
            cout << "Thread " << THIS_THREAD_ID << " failed to get name lock." << endl;
        } // if

        cout << "Thread " << THIS_THREAD_ID << " finished process." << endl;
    }

#if 0
    // This version is what we expected!
    void rwTest(int workTime)
    {
        auto deadline = boost::chrono::steady_clock::now() + boost::chrono::seconds(TIMEOUT);

        if (mtx.try_lock_shared_until(deadline)) {
            DLOG(INFO) << "Thread " << THIS_THREAD_ID << " got shared lock.";
            SLEEP_SECONDS(workTime);
            // get unique lock
            if (mtx.try_unlock_shared_and_lock_until(deadline)) {
                DLOG(INFO) << "Thread " << THIS_THREAD_ID << " working...";
                SLEEP_SECONDS(3);
                mtx.unlock_and_lock_shared();
                DLOG(INFO) << "Thread " << THIS_THREAD_ID << " job done.";
            } else {
                DLOG(INFO) << "Thread " << THIS_THREAD_ID << " failed to get unique lock.";
            } // if
            mtx.unlock_shared();
        } else {
            DLOG(INFO) << "Thread " << THIS_THREAD_ID << " failed to get shared lock.";
        } // if
    }
#endif

    void rwTest(int workTime)
    {
        auto deadline = boost::chrono::steady_clock::now() + boost::chrono::seconds(TIMEOUT);
        boost::upgrade_lock<SafeString> sLock(name, boost::defer_lock);

        if (sLock.try_lock_until(deadline)) {
            DLOG(INFO) << "Thread " << THIS_THREAD_ID << " got shared lock.";
            SLEEP_SECONDS(workTime);
            // get unique lock
            {
                DLOG(INFO) << "Thread " << THIS_THREAD_ID << " trying to get unique lock.";
                boost::upgrade_to_unique_lock<SafeString>  uLock(sLock);
                DLOG(INFO) << "Thread " << THIS_THREAD_ID << " working...";
                SLEEP_SECONDS(3);
                DLOG(INFO) << "Thread " << THIS_THREAD_ID << " job done.";
            }
        } else {
            DLOG(INFO) << "Thread " << THIS_THREAD_ID << " failed to get shared lock.";
        } // if
    }

    SafeString name;
    SafeString title;
    boost::shared_mutex mtx;
};


int main(int argc, char **argv)
{
    google::InitGoogleLogging(argv[0]);

    Foo foo;

    // std::thread thr1(std::bind(&Foo::process, &foo));
    // std::thread thr2(std::bind(&Foo::process, &foo));

    std::thread thr1(std::bind(&Foo::rwTest, &foo, 8));
    SLEEP_MILLISECONDS(10);
    std::thread thr2(std::bind(&Foo::rwTest, &foo, 3));

    thr1.join();
    thr2.join();

    cout << "All job done!" << endl;

    return 0;
}
