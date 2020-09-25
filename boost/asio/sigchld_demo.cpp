/*
c++ -o /tmp/test sigchld_demo.cpp -lboost_system -std=c++11 -pthread -g
 */
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <thread>
#include <boost/asio.hpp>
#include <unistd.h>
#include <sys/wait.h>

std::shared_ptr<boost::asio::io_service::work> pIoServiceWork;

void signal_handler(int signo, boost::asio::signal_set *signals)
{
    using namespace std;
    cout << "Got signal " << signo << endl;
    switch (signo) {
        case SIGINT:
        case SIGTERM:
            pIoServiceWork.reset();
            return;
        case SIGCHLD: {
                pid_t pid = 0;
                int stat = 0;
                while ((pid = ::waitpid(-1, &stat, WNOHANG)) > 0) {
                    cout << "Child process " << pid << " terminated with " << WEXITSTATUS(stat) << endl;
                }
            }
            break;
    }
    // NOTE!!! 持续信号处理,要重复调用
    signals->async_wait( [&](const boost::system::error_code& error, int signo)
            { signal_handler(signo, signals); } );
}

int main() {
    using namespace std;

    try {
        boost::asio::io_service                io_service;
        pIoServiceWork = std::make_shared< boost::asio::io_service::work >(std::ref(io_service));
        boost::asio::signal_set signals(io_service, SIGCHLD, SIGINT, SIGTERM);
        // signals_.add(SIGINT);
        // signals_.add(SIGTERM);
// #if defined(SIGQUIT)
        // signals_.add(SIGQUIT);
// #endif // defined(SIGQUIT)
        signals.async_wait( [&](const boost::system::error_code& error, int signo)
                { signal_handler(signo, &signals); } );

        cout << "Program running..." << endl;
        std::thread io_thread([&]{ io_service.run(); });
        // io_service.run();

        for (int i = 0; i < 10; ++i) {
            pid_t pid = fork();
            if (0 == pid) {
                ::srand(::time(0));
                int n = rand() % 10 + 1;
                cout << "Child process " << ::getpid() << " going to sleep " << n << " seconds" << endl;
                ::sleep(n);
                _exit(0);
            }
        }

        if (io_thread.joinable()) { io_thread.join(); }
        cout << "Program terminating..." << endl;

    } catch (const std::exception &ex) {
        cerr << "Exception in main: " << ex.what() << endl;
    } // try

    return 0;
}
