/*
 * c++ -o /tmp/test interrupt_demo.cpp -lboost_thread -lboost_system -std=c++11 -pthread -g
 */
#include <boost/thread.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include <boost/date_time.hpp>
#include <iostream>
#include <string>
#include <functional>
#include <signal.h>
// #include <pthread.h>

#define THIS_THREAD_ID        boost::this_thread::get_id()
#define SLEEP_SECONDS(x)      boost::this_thread::sleep_for(boost::chrono::seconds(x))
#define SLEEP_MILLISECONDS(x) boost::this_thread::sleep_for(boost::chrono::milliseconds(x))

using namespace std;

//!! interrupt 不能作用于 blocking I/O
static
void thr_func()
{
    // auto sig_int = [](int) {
        // cout << "Thread " << THIS_THREAD_ID << " received SIGINT" << endl;
    // };
    
    // sigset_t mask;
    // sigemptyset(&mask);
    // sigaddset(&mask, SIGINT);
    // pthread_sigmask( SIG_UNBLOCK, &mask, NULL );
    // signal( SIGINT, sig_int );   // signal 函数对全局都起作用

    while (true) {
        string line;
        getline( cin, line );
    } // while
}

// OK
static
void thr_func1()
{
    //!! pthread_t 和 get_id() 是一样的
    // cout << "Thread " << THIS_THREAD_ID << " running..." << endl;
    // cout << "Thread " << pthread_self() << " running..." << endl;
    
    while (true) {
        cout << "Thread " << THIS_THREAD_ID << " working..." << endl;
        SLEEP_MILLISECONDS(300);
    } // while
}


int main( int argc, char **argv )
{
    try {
        // sigset_t mask;
        // sigfillset( &mask );
        // sigprocmask( SIG_BLOCK, &mask, NULL );


        boost::thread thr1( thr_func );

        // use boost thread interrupt
        /*
         * SLEEP_SECONDS(3);
         * cout << "Trying to interrupt the thread..." << endl;
         * thr1.interrupt();
         * if (thr1.joinable())
         *     thr1.join();
         * cout << "Thread done!" << endl;
         */

        //!! NOTE!!
        /*
         * Signal dispositions are process-wide: if a signal handler is installed, 
         * the handler will be invoked in the thread thread, but if the  disposition
         * of the signal is "stop", "continue", or "terminate", this action will affect the whole process.
         */
        SLEEP_SECONDS(3);
        cout << "Trying to interrupt the thread..." << endl;
        pthread_t thrID = thr1.native_handle();
        pthread_kill(thrID, SIGINT);
        if (thr1.joinable())
            thr1.join();
        cout << "Thread done!" << endl;

        // pthread_cancel also doesn't work on blocking I/O
        /*
         * SLEEP_SECONDS(3);
         * cout << "Trying to interrupt the thread..." << endl;
         * pthread_t thrID = thr1.native_handle();
         * pthread_cancel( thrID );
         * if (thr1.joinable())
         *     thr1.join();
         * cout << "Thread done!" << endl;
         */

    } catch ( const std::exception &ex ) {
        cerr << ex.what() << endl;
    } // try

    return 0;
}
