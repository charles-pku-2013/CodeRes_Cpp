/*
 * c++ -o /tmp/test test.cpp -lboost_system -lboost_thread -lglog -std=c++11 -g
 */
#include <boost/asio.hpp>
#include <memory>
#include <chrono>
#include <functional>
#include <ctime>
#include <cstdlib>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <boost/chrono/chrono.hpp>
#include <glog/logging.h>


#define SLEEP_SECONDS(x)      boost::this_thread::sleep_for(boost::chrono::seconds(x))


class Test : public std::enable_shared_from_this<Test> {
public:
    Test(boost::asio::io_service& io_service)
            : io_service_(io_service), strand_(std::ref(io_service_)) {}

    boost::asio::io_service& io_service()
    { return io_service_; }

    void work()
    {
        // io_service_.post(std::bind(&Test::handle_work, shared_from_this()));
        io_service_.dispatch(std::bind(&Test::handle_work, shared_from_this()));
        // strand_.dispatch(std::bind(&Test::handle_work, shared_from_this()));
    }

    void handle_work()
    {
        // work();         // 🔴🔴NOTE!!! 若用dispatch，相当于无穷递归，core dump
        LOG(INFO) << "handle_work() running...";
        int sec = 1 + rand() % 5;
        SLEEP_SECONDS(sec);
        LOG(INFO) << "handle_work() done";
        work();         // 🔴🔴 放在这里用dispatch可以的，串行执行
    }

private:
    boost::asio::io_service&            io_service_; 
    boost::asio::io_service::strand     strand_;
};


int main()
{
    srand(time(0));

    boost::asio::io_service     io_service;
    std::unique_ptr<boost::asio::io_service::work> 
            pIoServiceWork(new boost::asio::io_service::work(std::ref(io_service)));

    const int N_WORKERS = 5;

    boost::thread_group thrgroup;
    for (int i = 1; i <= N_WORKERS; ++i)
        thrgroup.create_thread([&]{io_service.run();});

    auto p = std::make_shared<Test>(io_service);
    p->work();

    pIoServiceWork.reset();
    io_service.stop();
    thrgroup.join_all();

    return 0;
}

