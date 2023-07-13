/*
c++ -o /tmp/test signal_demo.cpp -lboost_system -std=c++11 -pthread -g
 */
#include <iostream>
#include <memory>
#include <boost/asio.hpp>

std::shared_ptr<boost::asio::io_service::work> pIoServiceWork;

void signal_handler(int signo)
{
    using namespace std;
    cout << "Got signal " << signo << endl;
    if (signo == SIGINT) {
        pIoServiceWork.reset();
    }
}

int main() {
    using namespace std;

    try {
        // When the io_service::run method is called without a work object,
        // it will return right away. Typically, that is not the behavior most developers are looking for.
        // There are of course some exceptions, but most developers are looking to specify a thread
        // to handle all of the asynchronous processing and don't want that thread to exit until told to do so.
        // That is what your code example does.
        // 这里可以不要io_service::work 如果信号处理函数的目的就是终止进程
        boost::asio::io_service                io_service;
        pIoServiceWork = std::make_shared< boost::asio::io_service::work >(std::ref(io_service));
        boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
        // signals_.add(SIGINT);
        // signals_.add(SIGTERM);
// #if defined(SIGQUIT)
        // signals_.add(SIGQUIT);
// #endif // defined(SIGQUIT)
        signals.async_wait( [](const boost::system::error_code& error, int signal)
                { signal_handler(signal); } );

        cout << "Program running..." << endl;
        io_service.run();

        cout << "Program terminating..." << endl;
        // pIoServiceWork.reset();

    } catch (const std::exception &ex) {
        cerr << "Exception in main: " << ex.what() << endl;
    } // try

    return 0;
}
