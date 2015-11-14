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

//!! 经过测试，当前实现并不能并发，一次只能服务一个client

#define SERVER_PORT             8888

using boost::asio::ip::tcp;
using std::cout; using std::endl;


std::string make_daytime_string()
{
    using namespace std; // For time_t, time and ctime;
    time_t now = time(0);
    return ctime(&now);
}


/* 
We will use shared_ptr and enable_shared_from_this because we want to 
keep the tcp_connection object alive as long as there is an operation that refers to it.
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

    void start() // tcp_server::handle_accept() 时，新客户来临，创建新连接，调用start()
    {
        for( int i = 0; i < 7; ++i ) {
            message_ = make_daytime_string();
            //!!   we call boost::asio::async_write() to serve the data to the client. Note that we are using boost::asio::async_write(), 
            //   rather than ip::tcp::socket::async_write_some(), to ensure that the entire block of data is sent.         	
            //!! 保证发送所有数据，相当于writen
            boost::asio::async_write(socket_, boost::asio::buffer(message_),
                    boost::bind(&tcp_connection::handle_write, shared_from_this(), //!! 调用本类的成员函数
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
            // boost::this_thread::sleep(boost::posix_time::seconds(3));
        } // for 
    }

private:
    tcp_connection(boost::asio::io_service& io_service)
        : socket_(io_service)
    {
    }

    //!! handle_write() 执行完了才向client发送EOF, client才退出
    void handle_write(const boost::system::error_code& /*error*/,
            size_t /*bytes_transferred*/)
    {
        cout << "tcp_connection::handle_write() " << boost::this_thread::get_id() << endl;
        boost::this_thread::sleep(boost::posix_time::seconds(3));
        cout << "tcp_connection::handle_write() end " << boost::this_thread::get_id() << endl;
    }

private:
    tcp::socket socket_;
    std::string message_;
};



class tcp_server
{
public:
    tcp_server(boost::asio::io_service& io_service)
        : acceptor_(io_service, tcp::endpoint(tcp::v4(), SERVER_PORT))           //!! 定义服务端口 port
    {
        start_accept();
    }

private:
    void start_accept() // 为每一个client分配一个connection,每一个client对应一个socket
    {
        cout << "tcp_server::start_accept() " << boost::this_thread::get_id() << endl;

        tcp_connection::pointer new_connection =
                tcp_connection::create(acceptor_.get_io_service());

        acceptor_.async_accept(new_connection->socket(),		//!! 这是并发实现
                boost::bind(&tcp_server::handle_accept, this, new_connection,
                    boost::asio::placeholders::error));
    }

   /*
    * The function handle_accept() is called when the asynchronous accept operation initiated by start_accept() finishes. 
    * It services the client request, and then calls start_accept() to initiate the next accept operation.
    */
    void handle_accept(tcp_connection::pointer new_connection,
            const boost::system::error_code& error)
    {
        if (!error)		// error = 0 OK
        {
            cout << "tcp_server::handle_accept() " << boost::this_thread::get_id() << " " << error << endl;
            new_connection->start();	// 开启服务流程, 应该放到单独线程中执行
        }

        start_accept();
    }

private:
    tcp::acceptor acceptor_;
};


int main()
{	
    try
    {
        // The io_service object provides I/O services, such as sockets, 
        // that the server object will use.
        // 这一个io_service要供多个socket使用，每accept一次，要生成一个新connection
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












