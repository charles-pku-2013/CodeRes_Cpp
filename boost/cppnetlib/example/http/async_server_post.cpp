
#include <vector>

#include <boost/config/warning_disable.hpp>
#include <boost/network/include/http/server.hpp>
#include <boost/network/utils/thread_pool.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

namespace net = boost::network;
namespace http = boost::network::http;
namespace utils = boost::network::utils;

struct async_hello_world;
typedef http::async_server<async_hello_world> server;

struct connection_handler : boost::enable_shared_from_this<connection_handler> {

    connection_handler(server::request const &request)
        :req(request), body("") {}

    ~connection_handler() {
        std::cout << "connection_handler dctor called!" << std::endl;
    }

    void operator()(server::connection_ptr conn) {
        int cl;
        server::request::headers_container_type const &hs = req.headers;
        for(server::request::headers_container_type::const_iterator it = hs.begin(); it!=hs.end(); ++it) {
            if(boost::to_lower_copy(it->name)=="content-length") {
                cl = boost::lexical_cast<int>(it->value);
                break;
            }
        }

        read_chunk(cl, conn);
    }

    void read_chunk(size_t left2read, server::connection_ptr conn) {
        std::cout << "left2read: " << left2read << std::endl;
        conn->read(
            boost::bind(
                &connection_handler::handle_post_read,
                connection_handler::shared_from_this(),
                _1, _2, _3, conn, left2read
                )
            );
    }

    void handle_post_read( server::connection::input_range range, 
            boost::system::error_code error, size_t size, server::conneststdft2read) 
    {
        if(!error) {
            std::cout << "read size: " << size << std::endl;
            body.append(boost::begin(range), size);
            size_t left = left2read - size;
            if(left>0) {
                read_chunk(left, conn);
            } else {
                //std::cout << "FINISHED at " << body.size()<< std::endl;
            }
                
        }
        std::cout << "error: " << error.message() << std::endl;
    }

    void handle_post_request(server::connection_ptr conn)
    {
        std::cout << "handle request..." << std::endl;
        std::cout << "post size: " << body.size() << std::endl;
    }

    server::request const &req;
    std::string body;
};


struct async_hello_world {

    void operator()(server::request const &request, server::connection_ptr conn) {
        boost::shared_ptr<connection_handler> h(new connection_handler(request));
        (*h)(conn);
    }

    void error(boost::system::error_code const & ec) {
        // do nothing here.
    std::cout << "async error: " << ec.message() << std::endl;
    }
};

int main(int argc, char * argv[]) {
    utils::thread_pool thread_pool(4); 
    async_hello_world handler;
    server instance("0.0.0.0", "1935", handler, thread_pool);
    instance.run();
    return 0;
}

