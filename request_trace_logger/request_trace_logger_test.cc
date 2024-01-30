#include "request_trace_logger.h"
#include <iostream>
#include <unordered_map>

using namespace KKK;

int main(int argc, char **argv) {
    auto conf = hocon::config::parse_string(
        "sample_rate : 5,"
        "logdir : /tmp/test_log_dir,"
        "rotation_time : \"00:00:00\","
        "rotation_size : 10m,"
        "max_log_files : 5,"
    );

    auto& logger_mgr = RequestTraceLoggerManager::Instance();
    if (!logger_mgr.Init(conf)) {
        return -1;
    }

    std::cerr << logger_mgr.DebugString() << std::endl;

    logger_mgr.AddCommonRecord("ip", "127.0.0.1");
    for (int i = 0; i < 20; ++i) {
        auto logger = logger_mgr.NewLogger();
        logger.AddRecord("Name", "charles");
        logger.AddRecord("age", 18);
        logger.AddRecord("score", 100.25);
        logger.AddRecord("count", i);
        // 由于设置了采样率可能会返回一个null logger, 调用其AddRecord不会有任何动作，也不会报错
        // 实际使用时根据具体情况判断是否是null logger
        if (!logger) {
            std::cerr << "Null logger for " << i << std::endl;
        }
    }

    logger_mgr.SetSampleRate(1);

    // test write map
    {
        auto logger = logger_mgr.NewLogger();
        std::unordered_map<std::string, std::string> m1 = {
            {"RED", "#FF0000"},
            {"GREEN", "#00FF00"},
            {"BLUE", "#0000FF"}
        };
        logger.AddRecord(m1.begin(), m1.end());

        std::unordered_map<std::string, int> m2 = {
            {"Foo", 10},
            {"Bar", 20},
            {"BAZ", 35}
        };
        logger.AddRecord(m2.begin(), m2.end(), "Test");
    }

    // test nested record
    {
        auto logger = logger_mgr.NewLogger();
        logger.AddRecord("info", "uuid", "cc177491cab0d175");
        logger.AddRecord("info", "pin", "jd_65f9b4c04a946");
        std::unordered_map<std::string, int> dict = {
            {"Beijing", 10},
            {"Shanghai", 21},
            {"Guangzhou", 20},
            {"Hongkong", 852},
        };
        logger.AddRecord("info", dict.begin(), dict.end());
    }

    // ::getchar();

    logger_mgr.Stop();  // Recommended explicitly call Stop before termination

    return 0;
}

#if 0
{"Name":"charles","age":18,"count":0,"ip":"127.0.0.1","score":100.25}
{"Name":"charles","age":18,"count":5,"ip":"127.0.0.1","score":100.25}
{"Name":"charles","age":18,"count":10,"ip":"127.0.0.1","score":100.25}
{"Name":"charles","age":18,"count":15,"ip":"127.0.0.1","score":100.25}
{"BLUE":"#0000FF","GREEN":"#00FF00","RED":"#FF0000","Test.BAZ":35,"Test.Bar":20,"Test.Foo":10,"ip":"127.0.0.1"}
{"info":{"Beijing":10,"Guangzhou":20,"Hongkong":852,"Shanghai":21,"pin":"jd_65f9b4c04a946","uuid":"cc177491cab0d175"},"ip":"127.0.0.1"}
#endif
