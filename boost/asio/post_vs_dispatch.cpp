/*
 * c++ -o /tmp/test test.cpp -lglog -lboost_thread -lboost_system -std=c++11 -g
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

// post vs. dispatch
/*
 * Well, it depends on the context of the call, i.e. is it run from within the io_service or without:
 * 
 * post will not call the function directly, ever, but postpone the call.
 * dispatch will call it rightaway if the dispatch-caller was called from io_service itself, but queue it otherwise.
 * So, it depends on the function calling post/dispatch was called, and if the given handler can be called straight away or not.
 * 
 * What this means:
 * 
 * ... is that dispatch might eventually call your code again (naturally, this depends on your app and how you chain calls), but in general you should make sure your code/object/function is re-entrant if you use dispatch.
 * 
 * dispatch is thus faster, as it avoids queueing the call if possible. It comes with some caveats, so you might want needs to use post occasionally, or always (if you want to play it safe and can afford it).
 */


boost::asio::io_service     g_io_service;

static
void run_io_service()
{
    LOG(INFO) << "IO thread " << THIS_THREAD_ID << " running...";
    g_io_service.run();
    LOG(INFO) << "IO thread " << THIS_THREAD_ID << " terminating...";
}

/*
 * NOTE!!! post dispatch 区别仅仅在io_service调用的工作函数里使用才看得出区别。
 * dispatch 只有start没有finish
 */
static
void job_func(int i)
{
    LOG(INFO) << "Job " << i << " started in thread " << THIS_THREAD_ID;
    SLEEP_SECONDS(2);
    // g_io_service.dispatch(std::bind(job_func, i+1));
    g_io_service.post(std::bind(job_func, i+1));
    LOG(INFO) << "Job " << i << " finished in thread " << THIS_THREAD_ID;
}

int main( int argc, char **argv )
{
    // google::InitGoogleLogging(argv[0]);

    LOG(INFO) << "main thread " << THIS_THREAD_ID << " running...";

    // 如果没有，io_service 执行完了队列中所有缓存的job就退出，在getchar()之前terminating
    boost::asio::io_service::work io_work( std::ref(g_io_service) );

    boost::thread_group thrgrp;

    for (int i = 0; i < 1; ++i)
        thrgrp.create_thread( run_io_service );

    SLEEP_SECONDS(1);

    g_io_service.dispatch( std::bind(job_func, 1) );

    getchar();
    LOG(INFO) << "Trying to stop io_service...";

    // NOTE!!! 如果job_func里用dispatch就停不下来，嵌套
    g_io_service.stop();

    thrgrp.join_all();

    return 0;
}

