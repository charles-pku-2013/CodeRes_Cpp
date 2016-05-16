#include <boost/asio.hpp>
#include <iostream>

using namespace std;

bool port_in_use(uint16_t port) 
{
    using namespace boost::asio;
    using ip::tcp;

    io_service svc;
    tcp::acceptor a(svc);

    boost::system::error_code ec;
    a.open(tcp::v4(), ec) || a.bind({ tcp::v4(), port }, ec);
    // 如果传入port是0，OS会随机挑选一个port如53338
    // cout << a.local_endpoint() << endl;

    return ec == error::address_in_use;
}

int main()
{
    try {
        // cout << port_in_use(9000) << endl;
        cout << port_in_use(0) << endl;

    } catch (const std::exception &ex) {
        cerr << ex.what() << endl;
    }

    return 0;
}

