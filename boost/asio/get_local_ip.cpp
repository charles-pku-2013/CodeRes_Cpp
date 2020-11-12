/*
 * c++ -o /tmp/test get_local_ip.cpp -lboost_system -std=c++11 -pthread -g
 */
#include <iostream>
#include <algorithm>
#include <thread>
#include <boost/asio.hpp>

// 同步调用
int main() {
    using boost::asio::ip::udp;

    try {
        boost::asio::io_service netService;
        udp::resolver   resolver(netService);
        udp::resolver::query query(udp::v4(), "www.baidu.com", "");
        udp::resolver::iterator endpoints = resolver.resolve(query), end;
        // TODO check != end  如果用域名方式
        // for (; endpoints != end; ++endpoints) {  // DNS返回的所有结果
            // udp::endpoint ep = *endpoints;  // 必须用这种方式，重载了 operator*
            // std::cout << ep << std::endl;
        // }
        // return 0;
        // udp::endpoint ep = *endpoints;
        // ep.port(52618);  // NOTE! 通过resolver得到的port都是0，这是不行的，connect会失败, 端口可以任意但不能是0

        // 直接用目标ip
        boost::asio::ip::address remote_addr = boost::asio::ip::address::from_string("8.8.8.8");
        udp::endpoint ep(remote_addr, 52618);  // 端口任意
        std::cout << ep << std::endl;
        udp::socket socket(netService);
        socket.connect(ep);
        std::cout << "Local endpoint: " << socket.local_endpoint() << std::endl;  // 带端口号，客户端是随机的
        boost::asio::ip::address addr = socket.local_endpoint().address();
        std::cout << "My IP is: " << addr.to_string() << std::endl;  // 只有ip，去掉端口
    } catch (std::exception& e){
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
