#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <deque>
#include <iostream>
#include <string>

// If you call write before the first write has completed it will send the same data again. 

class Connection
{
public:
    Connection(
            boost::asio::io_service& io_service
            ) :
        _io_service( io_service ),
        _strand( _io_service ),
        _socket( _io_service ),
        _outbox()
    {

    }

    void write( 
            const std::string& message
            )
    {
        /*
         * This function is used to ask the strand to execute the given handler, 
         * but without allowing the strand to call the handler from inside this function.
         * The strand object guarantees that handlers posted or dispatched through the strand 
         * will not be executed concurrently.
         */
        _strand.post(
                boost::bind(
                    &Connection::writeImpl,
                    this,
                    message
                    )
                );
    }

private:
    void writeImpl(
            const std::string& message
            )
    {
        // 保证对outbox读写操作按序进行，每次发起写操作也是
        _outbox.push_back( message );
        if ( _outbox.size() > 1 ) {
            // outstanding async_write
            return;
        }

        this->write();
    }

    void write()
    {
        /*
         * wrap(): Create a new handler that automatically dispatches the wrapped handler on the strand.
         */
        const std::string& message = _outbox[0];
        boost::asio::async_write(
                _socket,
                boost::asio::buffer( message.c_str(), message.size() ),
                _strand.wrap(
                    boost::bind(
                        &Connection::writeHandler,
                        this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred
                        )
                    )
                );
    }

    void writeHandler(
            const boost::system::error_code& error,
            const size_t bytesTransferred
            )
    {
        _outbox.pop_front();

        if ( error ) {
            std::cerr << "could not write: " << boost::system::system_error(error).what() << std::endl;
            return;
        }

        if ( !_outbox.empty() ) {
            // more messages to send
            this->write();
        }
    }


private:
    typedef std::deque<std::string> Outbox;

private:
    boost::asio::io_service& _io_service;
    boost::asio::io_service::strand _strand;
    boost::asio::ip::tcp::socket _socket;
    Outbox _outbox; // 对_outbox操作都被strand序列化了，所以不存在竞争问题，不需要上锁。
};

int
main()
{
    boost::asio::io_service io_service;
    Connection foo( io_service );

    return 0;
}





class Connection : public std::enable_shared_from_this<Connection> {
public:
    virtual void write(const std::string& data) {
        std::lock_guard<std::mutex> l(lock_);
        bool triggerSend = buffer_.size() == 0;
        // add data to buffer
        if (triggerSend) {
            do_send_chunk();
        }
    }

    void on_written(const boost::system::error_code& e, std::size_t length) {
        if (e) {
            // handle error here
            return;
        }

        std::lock_guard<std::mutex> l(lock_);
        buffer_.consume(length);
        if (buffer_.size() > 0) { // still has data to send
            do_send_chunk();
        }
    }

private:
    void do_send_chunk() {
        socket_->async_write_some(boost::asio::buffer(buffer_.data(),
            buffer_.size()),
            std::bind(&Connection::on_written,
            shared_from_this(),
            std::placeholders::_1,
            std::placeholders::_2));
    }

    boost::asio::io_service& service_;
    std::unique_ptr<socket> socket_;

    boost::asio::streambuf buffer_;
    std::mutex lock_;
};

