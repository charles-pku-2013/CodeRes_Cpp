/*
 * sample application to show the usage of work queues along with async http
 *server
 *
 * (C) Copyright Dino Korah 2013.
 * Distributed under the Boost Software License, Version 1.0. (See copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

// #include <boost/network/utils/thread_group.hpp>
#include <boost/network/include/http/server.hpp>
#include <boost/network/uri.hpp>

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <list>
#include <signal.h>

// This is needed to terminate the worker queue, and must be visible to the signal handler.
bool running = true;

struct handler;
typedef boost::network::http::async_server<handler> server;

struct server_data {
  boost::network::http::async_server<handler> server;

  server_data(const server::options &options)
    : server(options) {}

  void run() {
    server.run();
  }

  void stop() {
    running = false;
    server.stop();
  }
};

/**
 * request + connection encapsulation (work item)
 */
struct request_data {
  const server::request req;
  server::connection_ptr conn;

  typedef std::shared_ptr<request_data> pointer;

  /*
   * !! connection: server/async_connection.hpp  所有状态定义
   * read 已经用了 strand
   *  /// Type required for ``read`` callbacks. Takes an input range, an error
   * /// code, the number of bytes read, and a connection pointer.
   * typedef std::function<void(input_range, std::error_code, std::size_t,
   *                          connection_ptr)> read_callback_function;
   */
  request_data(const server::request &req, const server::connection_ptr &conn)
      : req(std::move(req)), conn(std::move(conn)) {}
};

/**
 * A basic work queue
 */
struct work_queue {
  typedef std::list<request_data::pointer> list;

  list requests;
  std::mutex mutex;

  inline void put(const request_data::pointer& request) {
    std::unique_lock<std::mutex> lock(mutex);
    requests.push_back(request);
    (void)lock;
  }

  inline request_data::pointer get() {
    std::unique_lock<std::mutex> lock(mutex);

    request_data::pointer request;
    if (!requests.empty()) {
      request = requests.front();
      requests.pop_front();
    }

    (void)lock;

    return request;
  }
};

struct handler {
  work_queue& queue;

  handler(work_queue& queue) : queue(queue) {}

  /**
   * 这是处理客户请求的主逻辑，加入待处理工作队列
   * Feed the work queue
   *
   * @param req
   * @param conn
   */
  void operator()(server::request const& req,
                  const server::connection_ptr& conn) {
    queue.put(std::make_shared<request_data>(req, conn));
  }
};

/**
 * Clean shutdown signal handler
 *
 * @param error
 * @param signal
 * @param server
 */
void shut_me_down(const std::error_code& error, int signal,
                  std::shared_ptr<server_data> server) {
  if (!error) server->stop();
}

/**
 * Process request; worker (thread)
 *
 * @param queue
 */
void process_request(work_queue& queue) {
    using namespace std;

    while (running) {
        request_data::pointer request(queue.get());
        if (request) {

            // server::request 源于 not_quite_pod_request_base 在 boost/network/protocol/http/impl/request.hpp
            cout << "Received client request from " << request->req.source << endl; // source 已经包含port
            cout << "destination = " << request->req.destination << endl;  // 去除了URL port
            cout << "method = " << request->req.method << endl;
            cout << "http version = " << (uint32_t)(request->req.http_version_major) 
                 << "." << (uint32_t)(request->req.http_version_minor) << endl;
            cout << "headers:" << endl;
            for (const auto &header : request->req.headers)
                cout << header.name << " = " << header.value << endl;
            //!! body always empty, how to read, see doc, http server : connection object read
            // cout << "body = " << request->req.body << endl;
            // cout << request->req.body.length() << " " << request->req.body.size() << endl;

            // boost::network::uri::uri url(request->req);

            // some heavy work!
            // std::this_thread::sleep_for(std::chrono::seconds(5));

            request->conn->set_status(server::connection::ok); // 枚举值见文档 http_server  Connection Object
            request->conn->write("Hello, world!");
        } // if

        std::this_thread::sleep_for(std::chrono::microseconds(1000)); // queue.get() 不是用条件变量实现的，队列空时候不会休眠
    } // while
}

  /**
   * Listens to the correct port and runs the server's event loop. This can be
   * run on multiple threads, as in the example below:
   *
   * Example:
   *    handler_type handler;
   *    http_server::options options(handler);
   *    options.thread_pool(
   *        std::make_shared<boost::network::utils::thread_pool>());
   *    http_server server(options.address("localhost").port("8000"));
   *
   *    // Run in three threads including the current thread.
   *    std::thread t1([&server] { server.run() });
   *    std::thread t2([&server] { server.run() });
   *    server.run();
   *    t1.join();
   *    t2.join();
   */

// 正确的结束方式: gracefully cleanly terminate
/*
 * 1. server->stop();
 * 2. io_service->stop();       g_pWork.reset(); g_pIoService->stop();
 * 3. threadgroup->join_all()
 */

int main() {
  try {
    // the thread group
    auto threads(std::make_shared<boost::network::utils::thread_group>());

    // setup asio::io_service
    auto io_service(std::make_shared<asio::io_service>());
    auto work(std::make_shared<asio::io_service::work>(std::ref(*io_service)));
 /*
  * * Some applications may need to prevent an io_service object's run() call from
  * * returning when there is no more work to do. For example, the io_service may
  * * be being run in a background thread that is launched prior to the
  * * application's asynchronous operations. The run() call may be kept running by
  * * creating an object of type asio::io_service::work:
  */

    // io_service threads  创建IO线程
    {
      int n_threads = 5;
      while (0 < n_threads--) {
        threads->create_thread([=] () { io_service->run(); });
      }
    }

    // the shared work queue
    work_queue queue;

    // 工作线程
    // worker threads that will process the request; off the queue
    {
      int n_threads = 5;
      while (0 < n_threads--) {
        threads->create_thread([&queue] () { process_request(queue); });
      }
    }

    // setup the async server
    handler request_handler(queue);
    auto server(std::make_shared<server_data>(
        server::options(request_handler)
        .address("0.0.0.0")
        .port("8000")
        .io_service(io_service)   // 通过 options 设置 io_service
        .reuse_address(true)
        .thread_pool(std::make_shared<boost::network::utils::thread_pool>(
             2, io_service, threads))));  // threads 中有工作线程和IO处理线程

    // setup clean shutdown
    asio::signal_set signals(*io_service, SIGINT, SIGTERM);
    signals.async_wait([=] (std::error_code const &ec, int signal) {
        shut_me_down(ec, signal, server);
      });

    // run the async server
    server->run();

    // 最后是12个线程，在本文件中创建了10个，在thread_pool构造中创建了2个
    // 本例有些问题
    std::cout << "n_threads in threadgroup: " << threads->size() << std::endl;

    work.reset();
    io_service->stop();

    threads->join_all();

    std::cout << "Terminated normally" << std::endl;
    exit(EXIT_SUCCESS);
  }
  catch (const std::exception& e) {
    std::cerr << "Abnormal termination - exception:" << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
}
