/*
 * c++ -o /tmp/test try_lock_until_test.cpp -lboost_thread -lboost_chrono -lboost_system -pthread -std=c++11 -g
 */
#include <boost/thread.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include <boost/chrono.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <functional>

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

    SafeString name;
    SafeString title;
};


int main()
{
    Foo foo;

    std::thread thr1(std::bind(&Foo::process, &foo));
    std::thread thr2(std::bind(&Foo::process, &foo));

    thr1.join();
    thr2.join();

    cout << "All job done!" << endl;

    return 0;
}
