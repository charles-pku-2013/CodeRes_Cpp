//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#define SERVER_PORT         8888

using boost::asio::ip::tcp;
using boost::asio::ip::address;

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }

        boost::asio::io_service io_service;

        // tcp::resolver resolver(io_service);
        // tcp::resolver::query query(argv[1], "daytime");


        // The list of endpoints is returned using an iterator of type ip::tcp::resolver::iterator. (Note that a default constructed ip::tcp::resolver::iterator object can be used as an end iterator.)
        // */	
        // tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        /* 
           Now we create and connect the socket. The list of endpoints obtained above may contain both IPv4 and IPv6 endpoints, so we need to try each of them until we find one that works. This keeps the client program independent of a specific IP version. The boost::asio::connect() function does this for us automatically.
           */	
        tcp::socket socket(io_service);
        tcp::endpoint endpoint(address::from_string(argv[1]), SERVER_PORT);
        socket.connect(endpoint);
        // boost::asio::connect(socket, endpoint_iterator);

        for (;;)
        {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            // The boost::asio::buffer() function automatically determines the size of the array to help prevent buffer overruns.
            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break; //!! Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            std::cout.write(buf.data(), len);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}












