/*
-lboost_system -pthread
kill -USR1 `ps aux | grep  '/tmp/test' | grep -ve 'grep' | awk '{print $2}'`
 */
#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include <unordered_map>
#include <thread>
#include <cstdio>
#include <boost/asio.hpp>
// #include "absl/memory/memory.h"
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

class SignalHandler final {
 public:
    using SigHandleFunc = std::function<void(void)>;
    using HandleMap = std::unordered_map<int, SigHandleFunc>;

 public:
    SignalHandler() : signals_(io_service_) {}

    explicit SignalHandler(int num_threads) : signals_(io_service_) {
        if (num_threads > 0) { n_handle_threads_ = num_threads; }
    }

    ~SignalHandler() {
        Stop();
        Wait();
    }

    void Start() {
        if (running_) { return; }
        running_ = true;
        // io_service_work_.reset(new boost::asio::io_service::work(std::ref(io_service_)));
        Wait();
        handle_threads_.clear();
        signals_.async_wait(std::bind(&SignalHandler::_SigHandleRoutine, this,
                    std::placeholders::_1, std::placeholders::_2));
        io_service_.restart();
        for (int i = 0; i < n_handle_threads_; ++i) {
            handle_threads_.emplace_back([this]{
                // unblock all signals
                sigset_t mask;
                sigemptyset(&mask);
                for (auto& kv : handle_map_) {
                    sigaddset(&mask, kv.first);
                }
                ::pthread_sigmask(SIG_UNBLOCK, &mask, NULL);
                io_service_.run();
            });
        }
    }

    void Stop() {
        if (!running_) { return; }
        running_ = false;
        signals_.cancel();
        // io_service_work_.reset();
    }

    void Wait() {
        for (auto& thr : handle_threads_) {
            if (thr.joinable())
            { thr.join(); }
        }
    }

    void Add(int signo, SigHandleFunc handle) {
        handle_map_[signo] = std::move(handle);
        signals_.add(signo);
        // block the signal in calling thread
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, signo);
        ::pthread_sigmask(SIG_BLOCK, &mask, NULL);  // TODO should backup old sigmask
    }

 private:
    void _SigHandleRoutine(const boost::system::error_code& error, int signo) {
        if (!running_ || error == boost::asio::error::operation_aborted) { return; }

        /*
         * NOTE!!! 必须先安排好后续处理，否则即使是多线程也只会用一个线程串行处理
         */
        signals_.async_wait(std::bind(&SignalHandler::_SigHandleRoutine, this,
                std::placeholders::_1, std::placeholders::_2));

        auto it = handle_map_.find(signo);
        if (it == handle_map_.end() || !(it->second)) { return; }
        try {
            (it->second)();
        } catch (const std::exception &ex) {

        } catch (...) {

        }
    }

    boost::asio::io_service io_service_;
    // std::unique_ptr<boost::asio::io_service::work> io_service_work_;
    boost::asio::signal_set signals_;
    HandleMap handle_map_;
    std::vector<std::thread> handle_threads_;
    int n_handle_threads_ = 1;
    bool running_ = false;
};

int main() {
    SignalHandler handler(5);

    auto sig_test = []{
        std::cout << "Thread " << std::this_thread::get_id() << " received C-c signal" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "Thread " << std::this_thread::get_id() << " finish C-c signal" << std::endl;
    };

    handler.Add(SIGINT, sig_test);
    handler.Start();
    std::cout << "SignalHandler running..." << std::endl;

    /*
     * NOTE!!! 由于SigHandler在独立的线程中运行，所以有必要在主线程中block由其处理的signal。
     * 否则信号会中断主线程的系统调用 如 sleep getchar, 除非主线程主动等待 SignalHandler 线程 (join)
     * pthread_sigmask 必须在创建子线程后调用，否则会影响子线程，子线程创建时会继承sigmask
     * https://devarea.com/linux-handling-signals-in-a-multithreaded-application/#.X6KKOkIzZTY
     * https://unix.stackexchange.com/questions/225687/what-happens-to-a-multithreaded-linux-process-if-it-gets-a-signal
     */
    // sigset_t mask;
    // sigemptyset(&mask);
    // sigaddset(&mask, SIGINT);
    // ::pthread_sigmask(SIG_BLOCK, &mask, NULL);

    ::getchar();

    std::cout << "Stop & Start" << std::endl;
    handler.Stop();
    handler.Start();
    handler.Wait();

    return 0;
}

#if 0
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
#endif


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
