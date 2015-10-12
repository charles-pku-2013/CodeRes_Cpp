// Synchronising handlers in multithreaded programs

/* 
calling io_service::run() from only one thread ensures that callback handlers cannot run concurrently.

have a pool of threads calling io_service::run(). However, as this allows handlers to execute concurrently, we need a method of synchronisation when handlers might be accessing a shared, thread-unsafe resource.

An boost::asio::strand guarantees that, for those handlers that are dispatched through it, an executing handler will be allowed to complete before the next one is started. This is guaranteed irrespective of the number of threads that are calling io_service::run(). 
 */
 
#include <unistd.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class printer
{
public:
  printer(boost::asio::io_service& io)
    : strand_(io),
      timer1_(io, boost::posix_time::seconds(1)),
      timer2_(io, boost::posix_time::seconds(1)),
      count_(0)
  {
	/* 	  
	When initiating the asynchronous operations, each callback handler is "wrapped" using the boost::asio::strand object. The strand::wrap() function returns a new handler that automatically dispatches its contained handler through the boost::asio::strand object. By wrapping the handlers using the same boost::asio::strand, we are ensuring that they cannot execute concurrently.
	 */	  
    timer1_.async_wait(strand_.wrap(boost::bind(&printer::print1, this)));
    timer2_.async_wait(strand_.wrap(boost::bind(&printer::print2, this)));
  }

  ~printer()
  {
    std::cout << "Final count is " << count_ << std::endl;
  }
  
  /* 
  In a multithreaded program, the handlers for asynchronous operations should be synchronised if they access shared resources. In this tutorial, the shared resources used by the handlers (print1 and print2) are std::cout and the count_ data member.
  两个print在一个线程中运行
   */
  void print1()
  {
    if (count_ < 10)
    {
		//!! 获取当前线程号 this_thread_id
      std::cout << "Timer 1: " << count_ << " in thread " << boost::this_thread::get_id() << std::endl;
	  // printf("Timer 1 pthread id = %lu\n", (long)pthread_self());
      ++count_;

      timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
      timer1_.async_wait(strand_.wrap(boost::bind(&printer::print1, this)));
    }
  }

  void print2()
  {
    if (count_ < 10)
    {
      std::cout << "Timer 2: " << count_ << " in thread " << boost::this_thread::get_id() << std::endl;
	  // printf("Timer 2 pthread id = %lu\n", (long)pthread_self());
      ++count_;

      timer2_.expires_at(timer2_.expires_at() + boost::posix_time::seconds(1));
      timer2_.async_wait(strand_.wrap(boost::bind(&printer::print2, this)));
    }
  }

private:
  boost::asio::io_service::strand strand_;
  boost::asio::deadline_timer timer1_;
  boost::asio::deadline_timer timer2_;
  int count_;
};

/* 
The main function now causes io_service::run() to be called from two threads: the main thread and one additional thread. This is accomplished using an boost::thread object.

Just as it would with a call from a single thread, concurrent calls to io_service::run() will continue to execute while there is "work" left to do. The background thread will not exit until all asynchronous operations have completed.
 */
int main()
{
  using namespace std;
  
  cout << "main thread " << boost::this_thread::get_id() << endl;
  // printf("main pthread id = %lu\n", (long)pthread_self());
  
  boost::asio::io_service io;
  printer p(io);
  boost::thread t(boost::bind(&boost::asio::io_service::run, &io));	//!! bind成员函数
  // 没有这一段整个程序都在一个线程中
  for( int i = 0; i < 6; ++i ) {
	  cout << "main thread waiting... " << i << endl;
	  sleep(1);
  } // for
    
  io.run();		// just waiting assigned "work" to finish.
  t.join();

  return 0;
}

/* 
main thread 7fd329806780
main thread waiting... 0
main thread waiting... 1
Timer 1: 0 in thread 7fd3281ed700
Timer 2: 1 in thread 7fd3281ed700
main thread waiting... 2
Timer 1: 2 in thread 7fd3281ed700
Timer 2: 3 in thread 7fd3281ed700
main thread waiting... 3
Timer 1: 4 in thread 7fd3281ed700
Timer 2: 5 in thread 7fd3281ed700
Timer 1: 6 in thread 7fd3281ed700
Timer 2: 7 in thread 7fd3281ed700
main thread waiting... 4
Timer 1: 8 in thread 7fd3281ed700
Timer 2: 9 in thread 7fd3281ed700
main thread waiting... 5
Final count is 10
 */




