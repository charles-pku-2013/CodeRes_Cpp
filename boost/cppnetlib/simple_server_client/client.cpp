/*
 * c++ -o /tmp/client client.cpp -lcppnetlib-client-connections -lcppnetlib-uri -lboost_system -lboost_thread -std=c++11 -pthread -g
 */

#include <iostream>
#include <boost/network/protocol/http/client.hpp>
// #include <boost/network/uri.hpp>
// #include <boost/network/uri/uri_io.hpp>


int main( int argc, char **argv )
{
    using namespace std;
    using namespace boost::network;
    using namespace boost::network::http;

    try {
        client::request request_("http://127.0.0.1:8000/");
        request_ << header("Connection", "close");
        client client_;
        client::response response_ = client_.get(request_);
        std::string body_ = body(response_);

        cout << "received response: " << body_ << endl;

    } catch ( const std::exception &ex ) {
        cerr << ex.what() << endl;
        exit(-1);
    } // try

    return 0;
}
