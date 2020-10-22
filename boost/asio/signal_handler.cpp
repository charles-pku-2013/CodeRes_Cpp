#pragma once
#include <functional>
#include <vector>
#include <memory>
#include <unordered_map>
#include <thread>
#include <boost/asio.hpp>
#include "absl/memory/memory.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

class SignalHandler final {
 public:
    using SigHandleFunc = std::function<void(void)>;
    using HandleMap = std::unordered_map<int, SigHandleFunc>;

 public:
    explicit SignalHandler(int num_threads = 5) {
        if (num_threads > 0) { n_handle_threads_ = num_threads; }
    }

    ~SignalHandler() {
        Stop();
        Wait();
    }

    void Start() {
        if (running_) { return; }
        running_ = true;
        io_service_work_ = absl::make_unique<boost::asio::io_service::work>(std::ref(io_service_));
        signals_ = absl::make_unique<boost::asio::signal_set>(io_service_);
        signals_->async_wait(std::bind(&SignalHandler::_SigHandleRoutine, this,
                    std::placeholders::_1, std::placeholders::_2));
        for (int i = 0; i < n_handle_threads_; ++i) {
            handle_threads_.emplace_back([&,this]{ io_service_.run(); });
        }
    }

    void Stop() {
        if (!running_) { return; }
        running_ = false;
        signals_->cancel();
        io_service_work_.reset();
    }

    void Wait() {
        for (auto& thr : handle_threads_) {
            if (thr.joinable())
            { thr.join(); }
        }
    }

    void Add(int signo, SigHandleFunc handle) {
        handle_map_[signo] = std::move(handle);
        signals_->add(signo);
    }

 private:
    void _SigHandleRoutine(const boost::system::error_code& error, int signo) {
        if (!running_) { return; }

        auto it = handle_map_.find(signo);
        if (it == handle_map_.end() || !(it->second)) { return; }
        try {
            (it->second)();
        } catch (const std::exception &ex) {

        } catch (...) {

        }

        if (running_) {
            signals_->async_wait(std::bind(&SignalHandler::_SigHandleRoutine, this,
                    std::placeholders::_1, std::placeholders::_2));
        }
    }

    boost::asio::io_service io_service_;
    std::unique_ptr<boost::asio::io_service::work> io_service_work_;
    std::unique_ptr<boost::asio::signal_set> signals_;
    HandleMap handle_map_;
    std::vector<std::thread> handle_threads_;
    int n_handle_threads_ = 5;
    bool running_ = false;
};

int main() {
    SignalHandler handler;

    auto on_child = []{
        pid_t pid = 0;
        int stat = 0;
        while ((pid = ::waitpid(-1, &stat, WNOHANG)) > 0) {
            std::cout << "Thread " << std::this_thread::get_id()
                << " Child process " << pid << " terminated with " << WEXITSTATUS(stat) << std::endl;
        }
    };

    auto on_stop = [&]{
        std::cout << "Thread " << std::this_thread::get_id() << " received stop signal" << std::endl;
        handler.Stop();
    };

    handler.Add(SIGCHLD, on_child);
    handler.Add(SIGINT, on_stop);
    handler.Add(SIGTERM, on_stop);
    handler.Start();

    for (int i = 0; i < 10; ++i) {
        pid_t pid = fork();
        if (0 == pid) {
            std::cout << "Child process " << ::getpid() << " running..." << std::endl;
            ::sleep(1);
            _exit(0);
        }
    }

    handler.Wait();

    return 0;
}


#if 0
// bazel BUILD
cc_library(
    name = "signal_handler",
    hdrs = ["signal_handler.h"],
    deps = [
        "@com_google_absl//absl/memory:memory",
        "@boost//:asio",
    ],
    visibility = ["//visibility:public"],
)
#endif
