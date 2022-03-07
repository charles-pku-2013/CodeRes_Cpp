/*
qvr_streamer.gflags
-work_dir=/share
-video_dir=qvr
-video_type=mp4
-record_file=qvr_streamed.txt
-shutdown_time=1800
-url=rtmp://192.168.50.3:1935/live/live
-stream_cmd=ffmpeg -i $file -vcodec libx264 -preset:v ultrafast -r 30 -g 60 -keyint_min 60 -sc_threshold 0 -b:v 2500k -maxrate 3000k -bufsize 5000k -sws_flags lanczos+accurate_rnd -acodec aac -b:a 96k -ar 48000 -ac 2 -f flv $url
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
DEFINE_string(video_type, "mp4", "video type");
DEFINE_string(record_file, "qvr_streamed.txt", "record file");
DEFINE_string(stream_cmd, "", "stream command");
DEFINE_string(url, "", "rtmp://...");
DEFINE_int32(shutdown_time, 1800, "wait time in seconds for shutdown if no more work");
DEFINE_int32(stream_interval, 0, "wait time in seconds for successive stream file");

class QVRStreamer final {
 public:
    using RecordList = std::set<std::string>;

 public:
    QVRStreamer() {
        video_dir_ = FLAGS_video_dir;
        video_type_ = FLAGS_video_type;
        record_file_ = FLAGS_record_file;
        stream_cmd_ = FLAGS_stream_cmd;
        url_ = FLAGS_url;
        shutdown_time_ = FLAGS_shutdown_time;
        stream_interval_ = FLAGS_stream_interval;
        last_streamed_ = std::chrono::high_resolution_clock::now();
    }

    void Run();

 private:
    void _LoadRecord();
    void _UpdateRecord();
    void _Stream(const std::string &file);

    std::string record_file_, video_dir_, video_type_, stream_cmd_, url_;
    RecordList record_;  // only filename like xx.mp4 not including ./
    int32_t shutdown_time_ = 0, stream_interval_ = 0;;
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

    // load record from file
    _LoadRecord();

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
            const std::string &fname = it.first;
            const auto &path = it.second;
            if (record_.count(fname) == 0) {
                LOG(INFO) << absl::StrFormat("Streaming video %s ...", fname);
                _Stream(path);
                record_.insert(fname);
                _UpdateRecord();
                last_streamed_ = std::chrono::high_resolution_clock::now();
                if (stream_interval_ > 0)
                { ::sleep(stream_interval_); }
            }
        }

        // check for shutdown
        auto now = std::chrono::high_resolution_clock::now();
        auto idle_time = std::chrono::duration_cast<std::chrono::seconds>(now - last_streamed_).count();
        if (idle_time > shutdown_time_) {
            LOG(INFO) << "Poweroff for idle...";
            tools::run_cmd("poweroff.sh", nullptr);
        }

        ::sleep(10);
    }  // while true
}

void QVRStreamer::_Stream(const std::string &file) {
    std::string out;
    std::string cmd = absl::StrReplaceAll(stream_cmd_, {{"$file", file}, {"$url", url_}});
    // LOG(INFO) << "Stream cmd: " << cmd;  // DEBUG
    int32_t status = tools::run_cmd(absl::StrCat(cmd, " 2>&1"), &out);
    if (status != 0) {
        LOG(ERROR) << "Stream fail: " << out;
    }
}

void QVRStreamer::_LoadRecord() {
    record_.clear();

    std::string line;
    std::ifstream ifs(record_file_, std::ios::in);
    if (!ifs) { return; }

    while (std::getline(ifs, line)) {
        boost::trim(line);
        if (line.empty()) { continue; }
        record_.insert(line);
    }

    return;
}

void QVRStreamer::_UpdateRecord() {
    std::ofstream ofs(record_file_, std::ios::out | std::ios::trunc);
    if (!ofs) {
        LOG(ERROR) << absl::StrFormat("Update record fail! fail to open %s for writting!", record_file_);
        return;
    }
    for (const auto& v : record_) {
        ofs << v << std::endl;
    }
    ofs << std::flush;
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
    LOG(INFO) << "========= QVR Streamer running at " << std::ctime(&now) << " =========";

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
    LOG(INFO) << "Stream URL: " << FLAGS_url;
    LOG(INFO) << "Shutdown time: " << FLAGS_shutdown_time;
    LOG(INFO) << "Stream interval: " << FLAGS_stream_interval;

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

