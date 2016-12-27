/*
 * c++ -o /tmp/test thread_pool.cpp -lglog -lboost_thread -lboost_system -std=c++11 -g
 */
#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/asio.hpp> 
#include <memory>
#include <functional>
#include <cstdio>
#include <glog/logging.h>

#define THIS_THREAD_ID        boost::this_thread::get_id()
#define SLEEP_SECONDS(x)      boost::this_thread::sleep_for(boost::chrono::seconds(x))
#define SLEEP_MILLISECONDS(x) boost::this_thread::sleep_for(boost::chrono::milliseconds(x))


class ThreadPool {
public:
    ThreadPool(std::size_t _N_Workers = 10, bool autoStart = true)
                : m_nWorkers(_N_Workers)
    { if (autoStart) start(); }

    virtual ~ThreadPool()
    try { stop(); } catch(...) {}

    void start()
    {
        if (!m_bRunning) {
            m_bRunning = true;
            m_pIoSrvWork.reset(new boost::asio::io_service::work(std::ref(m_IoService)));
            for (std::size_t i = 0; i < m_nWorkers; ++i)
                m_Thrgrp.create_thread([&, this]{
                        m_IoService.run(); });
        } // if
    }

    void stop()
    {
        if (m_bRunning) {
            m_pIoSrvWork.reset();
            m_Thrgrp.join_all();
            m_bRunning = false;
        } // if
    }

    void addThread(std::size_t n = 1)
    {
        m_nWorkers += n;
        if (m_bRunning) {
            for (std::size_t i = 0; i < n; ++i)
                m_Thrgrp.create_thread([&, this]{
                        m_IoService.run(); });
        } // if
    }

    template <typename Callable>
    void addWork(const Callable &work)
    { m_IoService.dispatch(work); }

protected:
    std::size_t                 m_nWorkers;
    bool                        m_bRunning;
    boost::asio::io_service     m_IoService;
    boost::thread_group         m_Thrgrp;
    std::unique_ptr<boost::asio::io_service::work>  m_pIoSrvWork;
};


static
void tp_routine(int i)
{
    int n = 10;
    while (n--) {
        LOG(INFO) << "tp_routine " << i << " running...";
        SLEEP_SECONDS(1);
    } // while
}

int main()
{
    ThreadPool tpl(5);

    for (int i = 1; i <= 10; ++i)
        tpl.addWork(std::bind(tp_routine, i));

    getchar();

    return 0;
}
