/*
 * c++ -o /tmp/test timer_demo.cpp -lboost_system -std=c++11 -pthread -g
 */
#include <iostream>
#include <memory>
#include <thread>
#include <boost/asio.hpp>
// #include <boost/date_time/posix_time/posix_time.hpp>


std::unique_ptr< boost::asio::deadline_timer >      g_Timer;


static
void signal_handler(int signo)
{
    using namespace std;
    g_Timer->cancel();
}

static
void timer_callback(const boost::system::error_code &ec)
{
    using namespace std;
    g_Timer->expires_from_now(boost::posix_time::seconds(1));
    g_Timer->async_wait(timer_callback);
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

        g_Timer.reset(new boost::asio::deadline_timer(std::ref(io_service)));

        g_Timer->expires_from_now(boost::posix_time::seconds(1));
        g_Timer->async_wait(timer_callback);

        io_service.run();

        pIoServiceWork.reset();

    } catch (const std::exception &ex) {
        cerr << "Exception in main: " << ex.what() << endl;
    } // try


    return 0;
}
