// async server
// compile: c++ -o server server.cpp -lboost_system -lboost_thread -lpthread -g

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

using boost::asio::ip::tcp;
using std::cout; using std::endl;


std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}


/* 
We will use shared_ptr and enable_shared_from_this because we want to keep the tcp_connection object alive as long as there is an operation that refers to it.
 */
class tcp_connection
  : public boost::enable_shared_from_this<tcp_connection>
{
public:
  typedef boost::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_service& io_service)
  {
    return pointer(new tcp_connection(io_service));
  }

  tcp::socket& socket()
  {
    return socket_;
  }

  void start()
  {
    message_ = make_daytime_string();
	
	/* 
	we call boost::asio::async_write() to serve the data to the client. Note that we are using boost::asio::async_write(), rather than ip::tcp::socket::async_write_some(), to ensure that the entire block of data is sent.
	 */	
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        boost::bind(&tcp_connection::handle_write, shared_from_this(), //!! 调用本类的成员函数
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

private:
  tcp_connection(boost::asio::io_service& io_service)
    : socket_(io_service)
  {
  }

  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
	  cout << "tcp_connection::handle_write()" << boost::this_thread::get_id() << endl;
	  boost::this_thread::sleep(boost::posix_time::seconds(5));	//!!?? 好像是单线程工作
  }

  tcp::socket socket_;
  std::string message_;
};


class tcp_server
{
public:
  tcp_server(boost::asio::io_service& io_service)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), 13))
  {
    start_accept();
  }

private:
  void start_accept()
  {
	cout << "tcp_server::start_accept() " << boost::this_thread::get_id() << endl;
	  
    tcp_connection::pointer new_connection =
      tcp_connection::create(acceptor_.get_io_service());

    acceptor_.async_accept(new_connection->socket(),		//!! 这是并发实现
        boost::bind(&tcp_server::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
  }
  
	/* 
	The function handle_accept() is called when the asynchronous accept operation initiated by start_accept() finishes. It services the client request, and then calls start_accept() to initiate the next accept operation.
	  */ 
  void handle_accept(tcp_connection::pointer new_connection,
      const boost::system::error_code& error)
  {
    if (!error)		// error = 0 OK
    {
	  cout << "tcp_server::handle_accept() " << boost::this_thread::get_id() << " " << error << endl;
      new_connection->start();	// 应该叫 sendmsg 
    }

    start_accept();
  }

  tcp::acceptor acceptor_;
};


int main()
{	
  try
  {
	// The io_service object provides I/O services, such as sockets, that the server object will use.	  
    boost::asio::io_service io_service;
    tcp_server server(io_service);
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}












