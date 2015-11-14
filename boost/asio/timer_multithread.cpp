// http://zh.highscore.de/cpp/boost/asio.html
// compile: c++ -o test timer_multithread.cpp -lboost_system -lboost_thread -g

#include <boost/asio.hpp> 
#include <boost/thread.hpp> 
#include <iostream> 

void handler1(const boost::system::error_code &ec) 
{ 
  std::cout << "handler1 5s in " << boost::this_thread::get_id() << std::endl; 
  boost::this_thread::sleep(boost::posix_time::seconds(1));
} 

void handler2(const boost::system::error_code &ec) 
{ 
  std::cout << "handler2 5s in " << boost::this_thread::get_id() << std::endl; 
  boost::this_thread::sleep(boost::posix_time::seconds(1));
} 

boost::asio::io_service io_service; 

//!! 多次调用同一个 I/O 服务的 run() 方法，是为基于 Boost.Asio 的应用程序增加可扩展性的推荐方法。
void run() 
{ 
  io_service.run(); 
} 

int main() 
{ 
  boost::asio::deadline_timer timer1(io_service, boost::posix_time::seconds(5)); 
  timer1.async_wait(handler1); 
  boost::asio::deadline_timer timer2(io_service, boost::posix_time::seconds(5)); 
  timer2.async_wait(handler2); 
  // 由于有两个线程，所以 handler1() 和 handler2() 可以同时执行。
  //  如果第二个计时器触发时第一个仍在执行，则第二个句柄就会在第二个线程中执行。
  //  如果第一个计时器的句柄已经终止，则 I/O 服务可以自由选择任一线程。
  boost::thread thread1(run); 
  boost::thread thread2(run); 
  thread1.join(); 
  thread2.join(); 

  return 0;
} 
