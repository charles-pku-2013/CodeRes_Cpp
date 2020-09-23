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
