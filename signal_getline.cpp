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


int main()
{
    using namespace std;

    try {
        boost::asio::io_service                io_service;
        auto pIoServiceWork = std::make_shared< boost::asio::io_service::work >(std::ref(io_service));
        boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
        signals.async_wait( [](const boost::system::error_code& error, int signal)
                { signal_handler(signal); } );
        std::thread thrIoSrv( [&]{ 
            io_service.run(); 
        } );

        /*
         * 按下 Ctrl-C 之后，getline 也会被中断
         */
        string line;
        while (getline(cin, line))
            cout << line << endl;

        pIoServiceWork.reset();
        thrIoSrv.join();

    } catch (const std::exception &ex) {
        cerr << "Exception caught in main: " << ex.what() << endl;
    } // try

    return 0;
}
