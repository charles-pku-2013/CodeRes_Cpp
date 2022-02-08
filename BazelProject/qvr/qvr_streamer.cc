#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include <unordered_set>
#include <chrono>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include "absl/strings/match.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_cat.h"
#include "tools/run_cmd.h"

namespace fs = boost::filesystem;

DEFINE_string(work_dir, "", "working directory");
DEFINE_string(video_dir, "qvr", "video directory");
DEFINE_string(video_type, "mp4", "video type");
DEFINE_string(record_file, "qvr_streamed.txt", "record file");
DEFINE_string(stream_cmd, "", "stream command");
DEFINE_int32(shutdown_time, 1800, "wait time for shutdown if no more work");

class QVRStreamer final {
 public:
    using RecordList = std::unordered_set<std::string>;

 public:
    QVRStreamer() {
        video_dir_ = FLAGS_video_dir;
        video_type_ = FLAGS_video_type;
        record_file_ = FLAGS_record_file;
        stream_cmd_ = FLAGS_stream_cmd;
        shutdown_time_ = FLAGS_shutdown_time;
    }

    void Run();

 private:
    void _LoadRecord();
    void _Stream(const std::string &file);

    std::string record_file_, video_dir_, video_type_, stream_cmd_;
    RecordList record_;  // only filename like xx.mp4 not including ./
    int32_t shutdown_time_ = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> last_streamed_;
};

void QVRStreamer::Run() {
    // load record from file
    _LoadRecord();

    while (true) {
        // Scan video dir
        fs::path video_path(video_dir_);
        if (!fs::exists(video_path) || !fs::is_directory(video_path)) {
            LOG(ERROR) << absl::StrFormat("Video dir %s does exist!", video_dir_);
            return;
        }

        std::map<std::string, std::string> file_list;
        for (fs::directory_iterator itr(video_path); itr != fs::directory_iterator(); ++itr) {
            if (!fs::is_regular_file(*itr)) { continue; }
            if (absl::EqualsIgnoreCase(itr->path().extension().string(), "." + video_type_)) {
                std::string fname = itr->path().filename().string();
                file_list[fname] = itr->path().string();
            }
        }  // for

        for (const auto& it : file_list) {
            const std::string &fname = it.first;
            const auto &path = it.second;
            if (record_.count(fname) == 0) {
                LOG(INFO) << absl::StrFormat("Streaming video %s ...", fname);
                _Stream(path);
                // std::string out;
                // int32_t status = tools::run_cmd(stream_cmd_, &out);
                // if (status != 0) {
                    // LOG(ERROR) << "Stream fail: " << out;
                // }
            }
        }

        ::sleep(10);
    }  // while
}

void QVRStreamer::_Stream(const std::string &file) {
    std::string out;
    std::string cmd = absl::StrReplaceAll(stream_cmd_, {{"$file", file}});
    int32_t status = tools::run_cmd(cmd, &out);
    if (status != 0) {
        LOG(ERROR) << "Stream fail: " << out;
    }
}

void QVRStreamer::_LoadRecord() {
    record_.clear();

    std::string line;
    std::ifstream ifs(record_file_, std::ios::in);
    if (!ifs) {
        return;
    }

    while (std::getline(ifs, line)) {
        boost::trim(line);
        if (line.empty()) { continue; }
        record_.insert(line);
    }

    return;
}

namespace {
void check_args() {
    if (FLAGS_stream_cmd.empty()) {
        LOG(ERROR) << "-stream_cmd not set!";
        exit(-1);
    }
}
}  // namespace

int main(int argc, char **argv)
try {
    GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = true;
    google::InitGoogleLogging(argv[0]);

    // set working dir
    if (!FLAGS_work_dir.empty()) {
        fs::current_path(FLAGS_work_dir);
    }

    check_args();

    LOG(INFO) << "Working dir: " << fs::current_path();
    LOG(INFO) << "Video dir: " << FLAGS_video_dir;
    LOG(INFO) << "Video type: " << FLAGS_video_type;
    LOG(INFO) << "Record file: " << FLAGS_record_file;
    LOG(INFO) << "Stream cmd: " << FLAGS_stream_cmd;

    QVRStreamer streamer;
    streamer.Run();

    return 0;

} catch (const std::exception &ex) {
    std::cerr << argv[0] << " terminated by exception: " << ex.what() << std::endl;
    return -1;
} catch (...) {
    std::cerr << argv[0] << " terminated by unknown exception!" << std::endl;
    return -1;
}

