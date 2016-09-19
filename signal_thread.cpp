#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <boost/asio.hpp>

#define THIS_THREAD_ID        std::this_thread::get_id()
#define SLEEP_SECONDS(x)      std::this_thread::sleep_for(std::chrono::seconds(x))
#define SLEEP_MILLISECONDS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))

/*
 * 信号处理函数在 io_service.run() 所在线程中执行
 */
static
void signal_handler(int signo)
{
    using namespace std;
    cout << "signal_handler run in thread " << THIS_THREAD_ID << " signo = " << signo << endl;
}

static
void test1()
{
    using namespace std;
    cout << "Test thread " << THIS_THREAD_ID << " running..." << endl;
    SLEEP_SECONDS(3);
    std::raise( SIGINT );    
    // raise 之后的语句还是会执行的。
    cout << "Test thread " << THIS_THREAD_ID << " raised signal." << endl;
}


int main()
{
    using namespace std;

    try {
        cout << "main thread " << THIS_THREAD_ID << " running..." << endl;

        boost::asio::io_service                io_service;
        auto pIoServiceWork = std::make_shared< boost::asio::io_service::work >(std::ref(io_service));
        boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
        signals.async_wait( [](const boost::system::error_code& error, int signal)
                { signal_handler(signal); } );
        std::thread thrIoSrv( [&]{ 
            cout << "io thread " << THIS_THREAD_ID << " running..." << endl;
            io_service.run(); 
        } );

        std::thread thrTest( test1 );
        thrTest.join();

        pIoServiceWork.reset();
        thrIoSrv.join();

    } catch (const std::exception &ex) {
        cerr << "Exception caught in main: " << ex.what() << endl;
    } // try

    return 0;
}
