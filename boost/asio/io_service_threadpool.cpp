/*
 * c++ -o /tmp/test io_service_threadpool.cpp -lglog -lboost_thread -lboost_system -std=c++11 -g
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
    // google::InitGoogleLogging(argv[0]);

    LOG(INFO) << "main thread " << THIS_THREAD_ID << " running...";

    // 如果没有，io_service 执行完了队列中所有缓存的job就退出，在getchar()之前terminating
    boost::asio::io_service::work io_work( std::ref(g_io_service) );

    boost::thread_group thrgrp;

    for (int i = 0; i < 5; ++i)
        thrgrp.create_thread( run_io_service );

    SLEEP_SECONDS(1);

    // for (int i = 1; i <= 7; ++i)
        // g_io_service.post( std::bind(job_func, i) );
    
    // here no different from post
    for (int i = 1; i <= 7; ++i)
        g_io_service.dispatch( std::bind(job_func, i) );

    // boost::asio::io_service::strand strand1(std::ref(g_io_service));
    // boost::asio::io_service::strand strand2(std::ref(g_io_service));
    // for (int i = 1; i <= 6; ++i) {
        // if (i % 2)
            // strand1.post( std::bind(job_func, i) );
        // else
            // strand2.post( std::bind(job_func, i) );
    // } // for

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


// post
// 典型的线程池，分配工作线程
/*
 * I1216 11:52:04.602571  2691 io_service_threadpool.cpp:40] main thread 7f8e9654c780 running...
 * I1216 11:52:04.603296  2694 io_service_threadpool.cpp:23] IO thread 7f8e93ad8700 running...
 * I1216 11:52:04.603332  2695 io_service_threadpool.cpp:23] IO thread 7f8e932d7700 running...
 * I1216 11:52:04.603356  2693 io_service_threadpool.cpp:23] IO thread 7f8e942d9700 running...
 * I1216 11:52:04.603373  2692 io_service_threadpool.cpp:23] IO thread 7f8e94ada700 running...
 * I1216 11:52:04.603407  2696 io_service_threadpool.cpp:23] IO thread 7f8e92ad6700 running...
 * I1216 11:52:05.604615  2694 io_service_threadpool.cpp:31] Job 1 started in thread 7f8e93ad8700
 * I1216 11:52:05.604657  2693 io_service_threadpool.cpp:31] Job 5 started in thread 7f8e942d9700
 * I1216 11:52:05.604617  2692 io_service_threadpool.cpp:31] Job 3 started in thread 7f8e94ada700
 * I1216 11:52:05.604626  2696 io_service_threadpool.cpp:31] Job 4 started in thread 7f8e92ad6700
 * I1216 11:52:05.604615  2695 io_service_threadpool.cpp:31] Job 2 started in thread 7f8e932d7700
 * I1216 11:52:10.605057  2695 io_service_threadpool.cpp:33] Job 2 finished in thread 7f8e932d7700
 * I1216 11:52:10.605057  2693 io_service_threadpool.cpp:33] Job 5 finished in thread 7f8e942d9700
 * I1216 11:52:10.605232  2693 io_service_threadpool.cpp:31] Job 7 started in thread 7f8e942d9700
 * I1216 11:52:10.605134  2695 io_service_threadpool.cpp:31] Job 6 started in thread 7f8e932d7700
 * I1216 11:52:10.605406  2694 io_service_threadpool.cpp:33] Job 1 finished in thread 7f8e93ad8700
 * I1216 11:52:10.605423  2692 io_service_threadpool.cpp:33] Job 3 finished in thread 7f8e94ada700
 * I1216 11:52:10.605470  2696 io_service_threadpool.cpp:33] Job 4 finished in thread 7f8e92ad6700
 * I1216 11:52:15.606465  2695 io_service_threadpool.cpp:33] Job 6 finished in thread 7f8e932d7700
 * I1216 11:52:15.606506  2693 io_service_threadpool.cpp:33] Job 7 finished in thread 7f8e942d9700
 * 
 * I1216 11:52:23.557356  2694 io_service_threadpool.cpp:25] IO thread 7f8e93ad8700 terminating...
 * I1216 11:52:23.557406  2692 io_service_threadpool.cpp:25] IO thread 7f8e94ada700 terminating...
 * I1216 11:52:23.557446  2695 io_service_threadpool.cpp:25] IO thread 7f8e932d7700 terminating...
 * I1216 11:52:23.557505  2696 io_service_threadpool.cpp:25] IO thread 7f8e92ad6700 terminating...
 * I1216 11:52:23.557539  2693 io_service_threadpool.cpp:25] IO thread 7f8e942d9700 terminating...
 */

