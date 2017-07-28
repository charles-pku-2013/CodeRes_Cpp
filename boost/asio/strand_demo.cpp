/*
 * c++ -o /tmp/test test.cpp -lboost_system -lboost_thread -lglog -std=c++11 -g
 */
/*
 * https://stackoverflow.com/questions/16397540/when-do-i-have-to-use-boostasiostrand
 * 
 * Broadly simplificated, an io_service contains a list of function objects (handlers). 
 * Handlers are put into the list when post() is called on the service. 
 * e.g. whenever an asynchronous operation completes, the handler and its arguments are put into the list.  
 * io_service::run() executes one handler after another. 
 * So if there is only one thread calling run() like in your case, there are no synchronisation problems 
 * and no strands are needed.
 * Only if multiple threads call run() on the same io_service, multiple handlers will be executed 
 * at the same time, in N threads up to N concurrent handlers. If that is a problem, e.g. 
 * if there might be two handlers in the queue at the same time that access the same object, 
 * you need the strand.
 * You can see the strand as a kind of lock for a group of handlers. 
 * If a thread executes a handler associated to a strand, that strand gets locked, 
 * and it gets released after the handler is done. Any other thread can execute only handlers 
 * that are not associated to a locked strand.
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
        // io_service_.post(std::bind(&Test::handle_work, shared_from_this())); // note 1,2
        strand_.post(std::bind(&Test::handle_work, shared_from_this()));    // note 3
    }

    void handle_work()
    {
        work();         // note 1 io_service.post()  多线程执行handler
        LOG(INFO) << "handle_work() running...";
        int sec = 1 + rand() % 5;
        SLEEP_SECONDS(sec);
        LOG(INFO) << "handle_work() done";
        // work();         // note 2  io_service.post()  单线程执行
    }

private:
    boost::asio::io_service&            io_service_; 
    boost::asio::io_service::strand     strand_;
};


int main()
{
    srand(time(0));

    boost::asio::io_service     io_service;
    boost::asio::io_service::work work(io_service);

    const int N_WORKERS = 5;

    boost::thread_group thrgroup;
    for (int i = 1; i <= N_WORKERS; ++i)
        thrgroup.create_thread([&]{io_service.run();});

    auto p = std::make_shared<Test>(io_service);
    p->work();

    io_service.stop();
    thrgroup.join_all();

    return 0;
}

#if 0
// note 1 io_service.post() 多线程执行handler
I0727 16:18:14.900136 1064960 test.cpp:37] handle_work() running...
I0727 16:18:14.900144 528384 test.cpp:37] handle_work() running...
I0727 16:18:14.900162 2138112 test.cpp:37] handle_work() running...
I0727 16:18:14.900177 2674688 test.cpp:37] handle_work() running...
I0727 16:18:14.900171 1601536 test.cpp:37] handle_work() running...
I0727 16:18:15.906419 2674688 test.cpp:40] handle_work() done
I0727 16:18:15.906530 2674688 test.cpp:37] handle_work() running...
I0727 16:18:16.906098 2138112 test.cpp:40] handle_work() done
I0727 16:18:16.906098 1601536 test.cpp:40] handle_work() done
I0727 16:18:16.906301 2138112 test.cpp:37] handle_work() running...
I0727 16:18:16.906488 1601536 test.cpp:37] handle_work() running...
I0727 16:18:17.901645 1064960 test.cpp:40] handle_work() done
I0727 16:18:17.901712 1064960 test.cpp:37] handle_work() running...
I0727 16:18:18.905421 528384 test.cpp:40] handle_work() done
I0727 16:18:18.905483 528384 test.cpp:37] handle_work() running...
I0727 16:18:18.911448 2674688 test.cpp:40] handle_work() done
I0727 16:18:18.911453 1601536 test.cpp:40] handle_work() done
#endif

#if 0
// note 2,3 io_service.post() // handler在主要工作完成后再次调用work(), 可实现串行执行handler
//          strand.post()  handler一开始调用work() 也是串行
I0727 16:22:05.408413 528384 test.cpp:37] handle_work() running...
I0727 16:22:10.413066 528384 test.cpp:40] handle_work() done
I0727 16:22:10.413152 528384 test.cpp:37] handle_work() running...
I0727 16:22:11.416486 528384 test.cpp:40] handle_work() done
I0727 16:22:11.416563 528384 test.cpp:37] handle_work() running...
I0727 16:22:13.421699 528384 test.cpp:40] handle_work() done
I0727 16:22:13.421830 528384 test.cpp:37] handle_work() running...
I0727 16:22:15.423672 528384 test.cpp:40] handle_work() done
I0727 16:22:15.423789 528384 test.cpp:37] handle_work() running...
I0727 16:22:16.426813 528384 test.cpp:40] handle_work() done
I0727 16:22:16.426882 528384 test.cpp:37] handle_work() running...
I0727 16:22:21.430158 528384 test.cpp:40] handle_work() done
I0727 16:22:21.430230 528384 test.cpp:37] handle_work() running...
I0727 16:22:22.432102 528384 test.cpp:40] handle_work() done
I0727 16:22:22.432190 528384 test.cpp:37] handle_work() running...
I0727 16:22:25.432520 528384 test.cpp:40] handle_work() done
#endif


