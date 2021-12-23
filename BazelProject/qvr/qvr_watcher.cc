#include <unistd.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include "absl/strings/match.h"
#include "absl/strings/str_format.h"

namespace fs = boost::filesystem;

DEFINE_string(work_dir, "", "working directory");
DEFINE_string(video_dir, "qvr", "video directory");
DEFINE_string(video_type, "mp4", "video type");
DEFINE_string(record_file, "qvr_streamed.txt", "record file");

class QVRWatcher final {
 public:
    using RecordList = std::unordered_set<std::string>;

 public:
    void SetRecordFile(const std::string &val)
    { record_file_ = val; }
    void SetVideoDir(const std::string &val)
    { video_dir_ = val; }
    void SetVideoType(const std::string &val)
    { video_type_ = val; }

    void Run();

 private:
    bool _LoadRecord();

    std::string record_file_, video_dir_, video_type_;
    RecordList record_;
};

void QVRWatcher::Run() {
    // load record from file
    if (!_LoadRecord()) { return; }

    // Scan video dir
    fs::path video_path(video_dir_);
    if (!fs::exists(video_path) || !fs::is_directory(video_path)) {
        LOG(ERROR) << absl::StrFormat("Video dir %s does exist!", video_dir_);
        return;
    }

    for (fs::directory_iterator itr(video_path); itr != fs::directory_iterator(); ++itr) {
        if (!fs::is_regular_file(*itr)) { continue; }
        if (absl::EqualsIgnoreCase(itr->path().extension().string(), "." + video_type_)) {
            std::string fname = itr->path().filename().string();
            if (record_.count(fname) == 0) {
                LOG(INFO) << "Found new video " << fname;
                // TODO wake up worker
            }
        }
    }  // for
}

bool QVRWatcher::_LoadRecord() {
    record_.clear();

    std::string line;
    std::ifstream ifs(record_file_, std::ios::in);
    if (!ifs) {
        LOG(ERROR) << "Fail to open record file " << record_file_;
        return false;
    }

    while (std::getline(ifs, line)) {
        boost::trim(line);
        if (line.empty()) { continue; }
        record_.insert(line);
    }

    return true;
}

int main(int argc, char **argv)
try {
    GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = true;
    google::InitGoogleLogging(argv[0]);

    // set working dir
    if (!FLAGS_work_dir.empty()) {
        fs::current_path(FLAGS_work_dir);
        LOG(INFO) << "Setting working dir to " << fs::current_path();
    }

    LOG(INFO) << "Video dir: " << FLAGS_video_dir;
    LOG(INFO) << "Video type: " << FLAGS_video_type;
    LOG(INFO) << "Record file: " << FLAGS_record_file;

    QVRWatcher watcher;
    watcher.SetVideoDir(FLAGS_video_dir);
    watcher.SetRecordFile(FLAGS_record_file);
    watcher.SetVideoType(FLAGS_video_type);

    while (true) {
        watcher.Run();
        ::sleep(10);
    }

    return 0;

} catch (const std::exception &ex) {
    std::cerr << argv[0] << " terminated by exception: " << ex.what() << std::endl;
    return -1;
} catch (...) {
    std::cerr << argv[0] << " terminated by unknown exception!" << std::endl;
    return -1;
}

