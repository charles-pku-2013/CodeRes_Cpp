/*
 * c++ -o /tmp/test post_vs_dispatch.cpp -lglog -lboost_thread -lboost_system -std=c++11 -g
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




#if 0
template <typename Handler>
void task_io_service::dispatch(Handler& handler)
{
  if (thread_call_stack::contains(this))    // within io_service
  {
    fenced_block b(fenced_block::full);
    boost_asio_handler_invoke_helpers::invoke(handler, handler);    // call directly
  }
  else
  {
    // Allocate and construct an operation to wrap the handler.
    typedef completion_handler<Handler> op;
    typename op::ptr p = { boost::asio::detail::addressof(handler),
      boost_asio_handler_alloc_helpers::allocate(
        sizeof(op), handler), 0 };
    p.p = new (p.v) op(handler);

    BOOST_ASIO_HANDLER_CREATION((p.p, "io_service", this, "dispatch"));

    do_dispatch(p.p);   // insert to queue
    p.v = p.p = 0;
  }
}

void task_io_service::do_dispatch(
    task_io_service::operation* op)
{
  work_started();
  mutex::scoped_lock lock(mutex_);
  op_queue_.push(op);
  wake_one_thread_and_unlock(lock);
}

template <typename Handler>
void task_io_service::post(Handler& handler)
{
  bool is_continuation =
    boost_asio_handler_cont_helpers::is_continuation(handler);

  // Allocate and construct an operation to wrap the handler.
  typedef completion_handler<Handler> op;
  typename op::ptr p = { boost::asio::detail::addressof(handler),
    boost_asio_handler_alloc_helpers::allocate(
      sizeof(op), handler), 0 };
  p.p = new (p.v) op(handler);

  BOOST_ASIO_HANDLER_CREATION((p.p, "io_service", this, "post"));

  post_immediate_completion(p.p, is_continuation); // 总是插入队列，除非单线程
  p.v = p.p = 0;
}


void task_io_service::post_immediate_completion(
    task_io_service::operation* op, bool is_continuation)
{
#if defined(BOOST_ASIO_HAS_THREADS)
  if (one_thread_ || is_continuation)
  {
    if (thread_info* this_thread = thread_call_stack::contains(this))
    {
      ++this_thread->private_outstanding_work;
      this_thread->private_op_queue.push(op);
      return;
    }
  }
#else // defined(BOOST_ASIO_HAS_THREADS)
  (void)is_continuation;
#endif // defined(BOOST_ASIO_HAS_THREADS)

  work_started();
  mutex::scoped_lock lock(mutex_);
  op_queue_.push(op);
  wake_one_thread_and_unlock(lock);
}
#endif



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

    for (int i = 0; i < 5; ++i)
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

// dispatch with 5 io thread
// 只在一个线程中工作
/*
 * I1216 12:51:20.269004  3469 post_vs_dispatch.cpp:62] main thread 7f43a1ad0780 running...
 * I1216 12:51:20.269707  3470 post_vs_dispatch.cpp:39] IO thread 7f43a005e700 running...
 * I1216 12:51:20.269909  3472 post_vs_dispatch.cpp:39] IO thread 7f439f05c700 running...
 * I1216 12:51:20.269927  3471 post_vs_dispatch.cpp:39] IO thread 7f439f85d700 running...
 * I1216 12:51:20.269950  3473 post_vs_dispatch.cpp:39] IO thread 7f439e85b700 running...
 * I1216 12:51:20.269980  3474 post_vs_dispatch.cpp:39] IO thread 7f439e05a700 running...
 * I1216 12:51:21.270568  3470 post_vs_dispatch.cpp:51] Job 1 started in thread 7f43a005e700
 * I1216 12:51:23.271471  3470 post_vs_dispatch.cpp:51] Job 2 started in thread 7f43a005e700
 * I1216 12:51:25.272882  3470 post_vs_dispatch.cpp:51] Job 3 started in thread 7f43a005e700
 * I1216 12:51:27.274181  3470 post_vs_dispatch.cpp:51] Job 4 started in thread 7f43a005e700
 * I1216 12:51:29.275575  3470 post_vs_dispatch.cpp:51] Job 5 started in thread 7f43a005e700
 * I1216 12:51:31.277009  3470 post_vs_dispatch.cpp:51] Job 6 started in thread 7f43a005e700
 * I1216 12:51:33.278439  3470 post_vs_dispatch.cpp:51] Job 7 started in thread 7f43a005e700
 * I1216 12:51:35.279350  3470 post_vs_dispatch.cpp:51] Job 8 started in thread 7f43a005e700
 * 
 * I1216 12:51:35.669036  3469 post_vs_dispatch.cpp:77] Trying to stop io_service...
 * I1216 12:51:35.669183  3472 post_vs_dispatch.cpp:41] IO thread 7f439f05c700 terminating...
 * I1216 12:51:35.669240  3471 post_vs_dispatch.cpp:41] IO thread 7f439f85d700 terminating...
 * I1216 12:51:35.669294  3473 post_vs_dispatch.cpp:41] IO thread 7f439e85b700 terminating...
 * I1216 12:51:35.669348  3474 post_vs_dispatch.cpp:41] IO thread 7f439e05a700 terminating...
 * I1216 12:51:37.280418  3470 post_vs_dispatch.cpp:51] Job 9 started in thread 7f43a005e700
 * I1216 12:51:39.281849  3470 post_vs_dispatch.cpp:51] Job 10 started in thread 7f43a005e700
 * I1216 12:51:41.283244  3470 post_vs_dispatch.cpp:51] Job 11 started in thread 7f43a005e700
 */
