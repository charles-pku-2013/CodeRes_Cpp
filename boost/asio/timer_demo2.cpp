/*
 * c++ -o /tmp/test test.cpp -lglog -lboost_system -pthread -std=c++11 -g
 */
#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <boost/asio.hpp>
#include <glog/logging.h>

#define THIS_THREAD_ID        std::this_thread::get_id()
#define SLEEP_SECONDS(x)      std::this_thread::sleep_for(std::chrono::seconds(x))
#define SLEEP_MILLISECONDS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))

static bool     running = true;
static std::unique_ptr< boost::asio::deadline_timer >      g_Timer;


static
void on_timer(const boost::system::error_code &ec)
{
    if (running) {
        LOG(INFO) << "on_timer set new timer.";
        g_Timer->expires_from_now(boost::posix_time::seconds(1));
        g_Timer->async_wait(on_timer);
    } // if

    LOG(INFO) << "on_timer called.";
    SLEEP_SECONDS(3);
}

int main(int argc, char **argv)
{
    using namespace std;

    boost::asio::io_service   io_service;

    // auto pIoServiceWork = std::make_shared< boost::asio::io_service::work >(std::ref(io_service));

    boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
    signals.async_wait( [&](const boost::system::error_code& error, int signal) { 
        running = false;
        // pIoServiceWork.reset();
        // io_service.stop();
    } );

    g_Timer.reset(new boost::asio::deadline_timer(std::ref(io_service)));
    g_Timer->expires_from_now(boost::posix_time::seconds(1));
    g_Timer->async_wait(on_timer);

    io_service.run();

    return 0;
}
