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

    for (int i = 1; i <= 7; ++i)
        g_io_service.post( std::bind(job_func, i) );

    getchar();

    g_io_service.stop();

    thrgrp.join_all();

    return 0;
}


