// Timer.2 - Using a timer asynchronously

/* 
Using asio's asynchronous functionality means having a callback function that will be called when an asynchronous operation completes. In this program we define a function called print to be called when the asynchronous wait finishes.

The asio library provides a guarantee that callback handlers will only be called from threads that are currently calling io_service::run(). Therefore unless the io_service::run() function is called the callback for the asynchronous wait completion will never be invoked.

The io_service::run() function will also continue to run while there is still "work" to do. In this example, the work is the asynchronous wait on the timer, so the call will not return until the timer has expired and the callback has completed.

It is important to remember to give the io_service some work to do before calling io_service::run(). For example, if we had omitted the above call to deadline_timer::async_wait(), the io_service would not have had any work to do, and consequently io_service::run() would have returned immediately.
 */
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using std::cout; using std::endl;

void print(const boost::system::error_code& /*e*/)
{
  std::cout << "Hello, world!" << std::endl;
}

int main()
{
  boost::asio::io_service io;

  boost::asio::deadline_timer t(io, boost::posix_time::seconds(3));
  t.async_wait(&print);
  
  // simulate doing other work, longer than the timer.
  for( size_t i = 0; i < 5; ++i ) {
	  cout << "waiting ... " << i << endl;
	  sleep(1);
  } // for 

  io.run();		// waiting for work to be done

  return 0;
}











