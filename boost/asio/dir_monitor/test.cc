#if 0
cc_library(
    name = "dir_monitor",
    hdrs = glob([
            "dir_monitor/**/*.hpp",
        ]
    ),
    deps = [
        "@boost//:asio",
        "@boost//:filesystem",
        "@boost//:system",
    ],
)
#endif

// 只能监控单层目录，对子目录操作事件都是0, 不宜使用wathcer
void TestDirMonitor(int argc, char **argv) {
    boost::asio::io_service io_service;
    // std::unique_ptr<boost::asio::io_service::work> io_service_work(new boost::asio::io_service::work(std::ref(io_service)));
    boost::asio::dir_monitor dm(io_service);

    std::function<void(const boost::system::error_code &ec, const boost::asio::dir_monitor_event &ev)> dir_change_handler;
    dir_change_handler =
            [&](const boost::system::error_code &ec, const boost::asio::dir_monitor_event &ev) {
        std::cout <<
            absl::StrFormat("Dir change, dirname: %s filename: %s event_type: %d",
                    ev.dirname, ev.filename, ev.type) << std::endl;
        dm.async_monitor(dir_change_handler);
    };

    for (int i = 1; i < argc; ++i) {
        dm.add_directory(argv[i]);
    }
    dm.async_monitor(dir_change_handler);
    io_service.run();
}
