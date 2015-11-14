#include <boost/asio.hpp> 
#include <string> 

boost::asio::io_service io_service; 
boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 80); 
boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint); 
boost::asio::ip::tcp::socket sock(io_service); 
std::string data = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, world!"; 

void write_handler(const boost::system::error_code &ec, std::size_t bytes_transferred) 
{ 
} 

//  boost::asio::ip::tcp::socket 还有一个名为 async_write_some() 的方法也可以发送数据；
//  不过它会在发送了至少一个字节之后调用相关联的句柄。 该句柄需要计算还剩余多少字节，
//  并反复调用 async_write_some() 直至所有字节发送完毕。 而使用 boost::asio::async_write() 
//  可以避免这些，因为这个异步操作仅在缓冲区的所有字节都被发送后才结束。
void accept_handler(const boost::system::error_code &ec) 
{ 
  if (!ec) 
  { 
    boost::asio::async_write(sock, boost::asio::buffer(data), write_handler); 
  } 
} 

int main() 
{ 
  acceptor.listen(); 
  acceptor.async_accept(sock, accept_handler); 
  io_service.run(); 
} 
