/*
 * c++ -o /tmp/server server.cpp -lcppnetlib-server-parsers -lcppnetlib-uri -lboost_system -lboost_thread -std=c++11 -pthread -g
 */

#include <iostream>
#include <boost/network/protocol/http/server.hpp>


namespace http = boost::network::http;

struct handler;
typedef http::server<handler> http_server;

struct handler {
    void operator() (const http_server::request &request,
                     http_server::response &response) 
    {
        response = http_server::response::stock_reply(
            http_server::response::ok, "Hello, world!");
    }

    void log(http_server::string_type const &info) 
    {
        std::cerr << "ERROR: " << info << '\n';
    }
};

int main(int arg, char * argv[]) 
{
    using namespace std;

    try {
        handler handler_;
        http_server::options options(handler_);
        http_server server_(
                options.address("0.0.0.0").port("8000"));
        cout << "Trying to start server..." << endl;
        server_.run();

    } catch (const std::exception &ex) {
        cerr << ex.what() << endl;
        exit(-1);
    } // try

    return 0;
}

