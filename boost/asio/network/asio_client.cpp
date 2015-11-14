#include <boost/asio.hpp> 
#include <boost/array.hpp> 
#include <iostream> 
#include <string> 

boost::asio::io_service io_service; 
boost::asio::ip::tcp::resolver resolver(io_service); 
boost::asio::ip::tcp::socket sock(io_service); 
boost::array<char, 4096> buffer; 

// 请留意，read_handler() 在将数据写出至 std::cout 之后，会再次调用 async_read_some() 方法。 
// 这是必需的，因为无法保证仅在一次异步操作中就可以接收到整个网页。
void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred) 
{ 
  if (!ec) 
  { 
    std::cout << std::string(buffer.data(), bytes_transferred) << std::endl; 
    sock.async_read_some(boost::asio::buffer(buffer), read_handler);  //!! 一直读，知道EOF
  } 
} 

void connect_handler(const boost::system::error_code &ec) 
{ 
  if (!ec) 
  { 
    boost::asio::write(sock, boost::asio::buffer("GET / HTTP 1.1\r\nHost: highscore.de\r\n\r\n")); 
    sock.async_read_some(boost::asio::buffer(buffer), read_handler); 
  } 
} 

//!! 域名解析
void resolve_handler(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it) 
{ 
  if (!ec) 
  { 
    sock.async_connect(*it, connect_handler); 
  } 
} 

int main() 
{ 
  boost::asio::ip::tcp::resolver::query query("www.highscore.de", "80"); 
  resolver.async_resolve(query, resolve_handler); 
  io_service.run(); 
} 
