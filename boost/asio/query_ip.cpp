/*
 * c++ -o /tmp/test query_ip.cpp -lboost_system -lglog -std=c++11 -pthread -g
 */
#include <iostream>
#include <string>
#include <iterator>
#include <memory>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <glog/logging.h>

#define THROW_RUNTIME_ERROR(x) \
    do { \
        std::stringstream __err_stream; \
        __err_stream << x; \
        __err_stream.flush(); \
        throw std::runtime_error( __err_stream.str() ); \
    } while (0)

using boost::asio::ip::tcp;
using boost::asio::ip::address;
using namespace std;

class IpQuery {
    typedef tcp::socket::endpoint_type  endpoint_type;
public:
    IpQuery( boost::asio::io_service& io_service,
             const std::string &svrAddr )
            : socket_(io_service)
    {
        string::size_type pos = svrAddr.find(':');
        if (string::npos == pos)
            THROW_RUNTIME_ERROR( svrAddr << " is not a valid address, must in format ip:addr" );

        uint16_t port = 0;
        if (!boost::conversion::try_lexical_convert(svrAddr.substr(pos+1), port) || !port)
            THROW_RUNTIME_ERROR("Invalid port number!");

        endpoint_type ep( address::from_string(svrAddr.substr(0, pos)), port );
        socket_.async_connect(ep, std::bind(&IpQuery::handle_connect,
                    this, std::placeholders::_1));
    }

private:
    //!! NOTE!!! 实际应用中，回调函数往往不在main线程中执行，所以都要放在 try...catch... 中
    void handle_connect(const boost::system::error_code& error)
    try {
        if( error ) {
            LOG(ERROR) << "fail to connect to " << socket_.remote_endpoint() << " error: " << error;
            return;
        } // if

        cout << "local ip is: " << socket_.local_endpoint().address() << endl;
    } catch( const std::exception &ex ) {
        LOG(ERROR) << "handle_connect() caught exception: " << ex.what();
    }


private:
    tcp::socket     socket_;
};


int main(int argc, char **argv)
{
    google::InitGoogleLogging(argv[0]);

    if (argc < 2) {
        cout << "Usage: " << argv[0] << " ip:port" << endl;
        return -1;
    } // if

    try {
        boost::asio::io_service io_service;

        IpQuery query(io_service, argv[1]);

        io_service.run();

    } catch (const std::exception &ex) {
        LOG(ERROR) << "Exception caught by main: " << ex.what();
    } // try

    return 0;
}
