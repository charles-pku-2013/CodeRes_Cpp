/*
bazel build -c opt //qvr:qvr_streamer
 */
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <ctime>
#include <chrono>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include "absl/strings/match.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_replace.h"
#include "tools/run_cmd.h"

namespace fs = boost::filesystem;

DEFINE_string(work_dir, "", "working directory");
DEFINE_string(video_dir, "qvr", "video directory");
DEFINE_string(download_dir, "", "directory for download stream finished videos");
DEFINE_string(video_type, "mp4", "video type");
DEFINE_string(stream_cmd, "", "stream command");
DEFINE_string(url, "", "rtmp://...");
DEFINE_int32(shutdown_time, 1800, "wait time in seconds for shutdown if no more work");
DEFINE_int32(stream_interval, 0, "wait time in seconds for successive stream file");
DEFINE_int32(try_cnt, 0, "try count if streaming fail");
DEFINE_int32(try_interval, 0, "time span between two try streaming if fail");

class QVRStreamer final {
 public:
    QVRStreamer() {
        video_dir_ = FLAGS_video_dir;
        download_dir_ = FLAGS_download_dir;
        video_type_ = FLAGS_video_type;
        stream_cmd_ = FLAGS_stream_cmd;
        url_ = FLAGS_url;
        shutdown_time_ = FLAGS_shutdown_time;
        stream_interval_ = FLAGS_stream_interval;
        last_streamed_ = std::chrono::high_resolution_clock::now();
        try_cnt_ = FLAGS_try_cnt;
        try_interval_ = FLAGS_try_interval;
    }

    void Run();

 private:
    bool _CheckMedia(const std::string &file);
    void _Stream(const std::string &file);
    void _MoveToDownload(const std::string &file);

    std::string video_dir_, download_dir_, video_type_, stream_cmd_, url_;
    int32_t shutdown_time_ = 0, stream_interval_ = 0, try_cnt_ = 0, try_interval_ = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> last_streamed_;
};

void QVRStreamer::Run() {
    // check video dir
    fs::path video_path(video_dir_);
    if (!fs::exists(video_path) || !fs::is_directory(video_path)) {
        LOG(ERROR) << absl::StrFormat("Video dir %s is not ready!", video_path.string());
        ::sleep(10);  // prepare for restart
        exit(0);
    }

    while (true) {
        // Scan video dir
        std::map<std::string, std::string> file_list;  // video file that scanned 文件名有序
        for (fs::directory_iterator itr(video_path); itr != fs::directory_iterator(); ++itr) {
            if (!fs::is_regular_file(*itr)) { continue; }
            if (absl::EqualsIgnoreCase(itr->path().extension().string(), "." + video_type_)) {
                std::string fname = itr->path().filename().string();
                file_list[fname] = itr->path().string();
            }
        }  // for

        // stream new found files
        for (const auto& it : file_list) {
            const auto &path = it.second;
            if (_CheckMedia(path)) {
                _Stream(path);
                _MoveToDownload(path);
                last_streamed_ = std::chrono::high_resolution_clock::now();
                if (stream_interval_ > 0)
                { ::sleep(stream_interval_); }
            }
        }

        // check for shutdown
        auto now = std::chrono::high_resolution_clock::now();
        auto idle_time = std::chrono::duration_cast<std::chrono::seconds>(now - last_streamed_).count();
        if (shutdown_time_ > 0 && idle_time > shutdown_time_) {
            LOG(INFO) << "Poweroff for idle...";
            tools::run_cmd("poweroff.sh", nullptr);
        }

        ::sleep(10);
    }  // while true
}

void QVRStreamer::_Stream(const std::string &file) {
    std::string cmd = absl::StrReplaceAll(stream_cmd_, {{"$file", file}, {"$url", url_}});
    // LOG(INFO) << "Stream cmd: " << cmd;
    std::string out;
    int32_t i = 0, status = 0;
    do {
        LOG(INFO) << absl::StrFormat("Streaming video %s ...", file);
        status = tools::run_cmd(absl::StrCat(cmd, " 2>&1"), &out);
        if (status == 0) {
            LOG(INFO) << "Successfully streamed video " << file;
            return;
        }
        LOG(ERROR) << absl::StrFormat("Stream %s fail! try again %d", file, (i + 1));
        if (try_interval_ > 0) { ::sleep(try_interval_); }
    } while (++i < try_cnt_);

    if (status != 0)
    { LOG(ERROR) << absl::StrFormat("Stream %s fail: %s", file, out); }
}

bool QVRStreamer::_CheckMedia(const std::string &file) {
    int32_t status = tools::run_cmd(absl::StrCat("ffprobe ", file, " 2>&1"), nullptr);
    // LOG(INFO) << absl::StrFormat("Check media %s %s!", file, (status ? "fail" : "success"));
    return (status == 0);
}

void QVRStreamer::_MoveToDownload(const std::string &old_p) {
    fs::path new_p = fs::path(download_dir_) / fs::path(old_p).filename();
    try {
        fs::create_directories(download_dir_);  // create the dir if not exists
        fs::rename(old_p, new_p);
    } catch (const std::exception &ex) {
        LOG(ERROR) << absl::StrFormat("Fail to move file from %s to %s, %s",
                        old_p, new_p.string(), ex.what());
    }
}

namespace {
void check_args() {
    if (FLAGS_stream_cmd.empty()) {
        LOG(ERROR) << "-stream_cmd not set!";
        exit(-1);
    }
    if (FLAGS_url.empty()) {
        LOG(ERROR) << "-url not set!";
        exit(-1);
    }
}
}  // namespace

int main(int argc, char **argv)
try {
    GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = true;
    google::InitGoogleLogging(argv[0]);

    std::time_t now = std::time(0);
    LOG(INFO) << "======== QVR Streamer running at " << std::ctime(&now) << " ========";

    // set working dir
    if (!FLAGS_work_dir.empty()) {
        fs::current_path(FLAGS_work_dir);
    }

    check_args();

    LOG(INFO) << "Working dir: " << fs::current_path();
    LOG(INFO) << "Video dir: " << FLAGS_video_dir;
    LOG(INFO) << "Download dir: " << FLAGS_download_dir;
    LOG(INFO) << "Video type: " << FLAGS_video_type;
    LOG(INFO) << "Stream cmd: " << FLAGS_stream_cmd;
    LOG(INFO) << "Stream URL: " << FLAGS_url;
    LOG(INFO) << "Shutdown time: " << FLAGS_shutdown_time;
    LOG(INFO) << "Stream interval: " << FLAGS_stream_interval;
    LOG(INFO) << "Try count: " << FLAGS_try_cnt;
    LOG(INFO) << "Try interval: " << FLAGS_try_interval;

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

