/*
 * How to gracefully end a connection:
 */

void handle_read(const boost::system::error_code& error,
        std::size_t bytes_transferred)
{
    // 🔴🔴这个error要单独判断
    if (boost::asio::error::eof == error)
    {
        shutdown_socket();
        stop();
        return;
    }

    if (error)
    {
        stop();
        return;
    }

    start_read_some();
}

boost::system::error_code shutdown_socket()
{
    boost::system::error_code error;
    socket_.shutdown(protocol::socket::shutdown_send, error);
    return error;
}

void stop()
{
    boost::system::error_code ignored;
    socket_.close(ignored);
}

// example 2
boost::system::error_code ec;
this->origin_server_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
this->origin_server_socket.close(ec);

// example 3
void close() noexcept {
    error_code ec;
    socket->lowest_layer().shutdown(asio::ip::tcp::socket::shutdown_both, ec);
    socket->lowest_layer().close(ec);
}



