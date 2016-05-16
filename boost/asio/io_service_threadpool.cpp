/*
 * c++ -o /tmp/test asio_demo.cpp -lglog -lboost_thread -lboost_system -std=c++11 -g
 * GLOG_logtostderr=1 /tmp/test
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

boost::asio::io_service     g_io_service;

static
void run_io_service()
{
    LOG(INFO) << "IO thread " << THIS_THREAD_ID << " running...";
    g_io_service.run();
    LOG(INFO) << "IO thread " << THIS_THREAD_ID << " terminating...";
}

static
void job_func(int i)
{
    LOG(INFO) << "Job " << i << " started in thread " << THIS_THREAD_ID;
    SLEEP_SECONDS(5);
    LOG(INFO) << "Job " << i << " finished in thread " << THIS_THREAD_ID;
}

int main( int argc, char **argv )
{
    google::InitGoogleLogging(argv[0]);

    // 如果没有，io_service 执行完了队列中所有缓存的job就退出，在getchar()之前terminating
    boost::asio::io_service::work io_work( std::ref(g_io_service) );

    boost::thread_group thrgrp;

    for (int i = 0; i < 5; ++i)
        thrgrp.create_thread( run_io_service );

    // for (int i = 1; i <= 7; ++i)
        // g_io_service.post( std::bind(job_func, i) );
    
    boost::asio::io_service::strand strand1(std::ref(g_io_service));
    boost::asio::io_service::strand strand2(std::ref(g_io_service));
    for (int i = 1; i <= 6; ++i) {
        if (i % 2)
            strand1.post( std::bind(job_func, i) );
        else
            strand2.post( std::bind(job_func, i) );
    } // for

    getchar();

    g_io_service.stop();

    thrgrp.join_all();

    return 0;
}

// output for using strand
// 同一个strand中的工作，保证在同一个线程中执行
/*
 * I0516 16:14:07.562134 1064960 io_service_threadpool.cpp:31] Job 1 started in thread 0x700000104000
 * I0516 16:14:07.562139 1601536 io_service_threadpool.cpp:31] Job 2 started in thread 0x700000187000
 * I0516 16:14:12.563966 1601536 io_service_threadpool.cpp:33] Job 2 finished in thread 0x700000187000
 * I0516 16:14:12.564036 1064960 io_service_threadpool.cpp:33] Job 1 finished in thread 0x700000104000
 * I0516 16:14:12.564085 1601536 io_service_threadpool.cpp:31] Job 4 started in thread 0x700000187000
 * I0516 16:14:12.564103 1064960 io_service_threadpool.cpp:31] Job 3 started in thread 0x700000104000
 * I0516 16:14:17.569026 1601536 io_service_threadpool.cpp:33] Job 4 finished in thread 0x700000187000
 * I0516 16:14:17.569032 1064960 io_service_threadpool.cpp:33] Job 3 finished in thread 0x700000104000
 * I0516 16:14:17.569133 1601536 io_service_threadpool.cpp:31] Job 6 started in thread 0x700000187000
 * I0516 16:14:17.569216 1064960 io_service_threadpool.cpp:31] Job 5 started in thread 0x700000104000
 * I0516 16:14:22.569931 1601536 io_service_threadpool.cpp:33] Job 6 finished in thread 0x700000187000
 * I0516 16:14:22.569938 1064960 io_service_threadpool.cpp:33] Job 5 finished in thread 0x700000104000
 */
