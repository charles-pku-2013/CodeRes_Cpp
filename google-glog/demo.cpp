/*
c++ -o /tmp/test google-glog/demo.cpp -lglog -lgflags -std=c++17 -DGLOG_USE_GLOG_EXPORT -g
/tmp/test -v=1
/tmp/test -v 2
 */
#include <glog/logging.h>
#include <gflags/gflags.h>
/*
 * link with -lglog
 *
 * env var opts:
 * GLOG_logtostderr=1 ./your_application
 * stderrthreshold
 * log_dir
 *
 * INFO, WARNING, ERROR, and FATAL.
 * Logging a FATAL message terminates the program (after the message is logged).
 */

/*
 * #error <glog/flags.h> was not included correctly. See the documentation for how to consume the library.
 * -DGLOG_USE_GLOG_EXPORT
 */

namespace {
/*
 *    glog settings:
 *    env                   arg
 *    GLOG_max_log_size   --max_log_size (MB)
 *    GLOG_log_dir        --log_dir
 *    GLOG_logtostderr    --logtostderr (全部输出到stderr不会生成log文件)
 *      <=> google::LogToStderr();
 */
// void init_log() {
//     google::InitGoogleLogging(FLAGS_server_name.c_str());
//     google::SetLogDestination(google::INFO, (FLAGS_server_log_path + "info_").c_str());
//     google::SetLogDestination(google::WARNING, (FLAGS_server_log_path + "warning_").c_str());
//     google::SetLogDestination(google::ERROR, (FLAGS_server_log_path + "error_").c_str());
//     if (FLAGS_debug) {
//         google::SetStderrLogging(google::INFO);  // 同时输出到stderr和log文件
//     } else {
//         google::SetStderrLogging(google::ERROR);
//     }
// }
}  // namespace

int main(int argc, char* argv[])
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    // Initialize Google's logging library.
    google::InitGoogleLogging(argv[0]);
    google::LogToStderr();

    // ...
    int num_cookies = 25;
    LOG(INFO) << "Found " << num_cookies << " cookies";

    VLOG(1) << "This message is logged when --v is 1 or higher.";
    VLOG(2) << "This message is logged when --v is 2 or higher.";
    VLOG(3) << "This message is logged when --v is 3 or higher.";

    return 0;
}
