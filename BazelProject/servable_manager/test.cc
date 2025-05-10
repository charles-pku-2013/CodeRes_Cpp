#include <iostream>
#include <thread>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include <boost/asio.hpp>
#include "demo_dict.h"
#include "servable_manager.h"

using namespace demo;
using namespace std;

void test(const std::string& name, int64_t version) {
    auto& mgr = ServableManager::Instance();
    cout << "ServableManager: " << mgr.DebugString() << endl;

    std::weak_ptr<DemoDict>   w_dict = mgr.GetServable<DemoDict>(name, version);
    std::shared_ptr<DemoDict> dict = w_dict.lock();

    if (dict) {
        cout << "Got dict: " << dict->Description() << endl;
    } else {
        cout << "Failed to get dict" << endl;
    }
}

int main(int argc, char **argv) {
    GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = true;
    google::InitGoogleLogging(argv[0]);

    // boost::asio::io_service                io_service;
    // auto pIoServiceWork = std::make_shared< boost::asio::io_service::work >(std::ref(io_service));
    // boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);

    auto& mgr = ServableManager::Instance();
    mgr.AddMonitorPath("testdata");

    // signals.async_wait([&](const boost::system::error_code& error, int signal) {
        // mgr.Stop();
        // pIoServiceWork.reset();
        // LOG(INFO) << "Program terminated";
    // });

    mgr.Start();

    std::string name;
    int64_t version = 0;
    LOG(INFO) << "Test begin...";
    while (cin >> name >> version) {
        test(name, version);
    }

    mgr.Stop();
    LOG(INFO) << "Test end";
    // io_service.run();

    return 0;
}

