/*
 * c++ -o /tmp/test get_local_ip.cpp -lboost_system -std=c++11 -pthread -g
 */
#include <iostream>
#include <algorithm>
#include <thread>
#include <boost/asio.hpp>
 
namespace ip = boost::asio::ip;

int main()
{
    using namespace ip;

    try {
        boost::asio::io_service netService;
        boost::asio::io_service::work work(netService);
        
        std::thread ioThr( [&]{ netService.run(); } );

        udp::resolver   resolver(netService);
        udp::resolver::query query(udp::v4(), "www.baidu.com", "");
        // udp::resolver::query query(udp::v4(), "google.com", "");
        udp::resolver::iterator endpoints = resolver.resolve(query);
        udp::endpoint ep = *endpoints;
        udp::socket socket(netService);
        socket.connect(ep);
        boost::asio::ip::address addr = socket.local_endpoint().address();
        std::cout << "My IP according to google is: " << addr.to_string() << std::endl;

        netService.stop();
        ioThr.join();
    } catch (const std::exception& e) {
        std::cerr << "Could not deal with socket. Exception: " << e.what() << std::endl;
    }

    return 0;
}

/*
 * int main() 
 * {
 *     boost::asio::io_service io_service;
 *     ip::tcp::resolver resolver(io_service);
 *  
 *     std::string h = ip::host_name();
 *     std::cout << "Host name is " << h << '\n';
 *     std::cout << "IP addresses are: \n";
 *     std::for_each(resolver.resolve({h, ""}), {}, [](const ip::tcp::endpoint& re) {
 *         std::cout << re.address() << '\n';
 *     });
 * 
 *     // 没有 io_service::work 支持，io_service发现工作队列中无内容会自动退出
 *     io_service.run();
 * 
 *     return 0;
 * }
 */
