/*
bazel build -c opt //multimedia:gen_chapter
 */
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include "absl/strings/str_format.h"
#include "absl/strings/str_cat.h"
#include <glog/logging.h>
#include <gflags/gflags.h>
#include "tools/run_cmd.h"

DEFINE_string(i, "", "media file");
DEFINE_string(c, "", "chapter file");
DEFINE_string(start, "Start", "title of the starting chapter");

namespace fs = boost::filesystem;

struct Chapter {
    Chapter(const std::string &_Title, int32_t _Start)
            : title(_Title), start(_Start) {}

    std::string title;
    int32_t start = 0, end = 0;

    bool operator<(const Chapter &rhs) const
    { return start < rhs.start; }

    std::string ToString() const {
        std::ostringstream ss;
        ss << "[CHAPTER]" << std::endl;
        ss << "TIMEBASE=1/1000" << std::endl;
        ss << "START=" << start << std::endl;
        ss << "END=" << end << std::endl;
        ss << "title=" << title << std::flush;
        return ss.str();
    }
};

class GenChapter final {
 public:
    using ChapterList = std::vector<Chapter>;

 public:
    void SetChapterFile(const std::string &file) { chapter_file_ = file; }
    void SetMediaFile(const std::string &file) { media_file_ = file; }
    void SetStartTitle(const std::string &title) { start_title_ = title; }
    void Run();

 private:
    void _ReadChapter();
    int32_t _GetDuration();
    int32_t _GetMeta();
    int32_t _MergeMeta();
    static int32_t _Str2Time(const std::string &time);

    std::string media_file_, chapter_file_, meta_file_, start_title_;
    ChapterList chapters_;
    int32_t duration_ = 0;
};

void GenChapter::Run() {
    chapters_.clear();

    meta_file_ = std::tmpnam(nullptr);
    if (meta_file_.empty()) {
        LOG(ERROR) << "Cannot create meta file!";
        exit(-1);
    }

    duration_ = _GetDuration();
    if (duration_ < 0) {
        exit(-1);
    }

    // Add start chapter
    chapters_.emplace_back(start_title_, 0);

    // Add following chapters from file
    _ReadChapter();

    // sort
    std::sort(chapters_.begin(), chapters_.end());

    // set end time
    for (int32_t i = 0; i < chapters_.size() - 1; ++i) {
        auto& chapter = chapters_[i];
        chapter.end = chapters_[i + 1].start;
    }
    auto& last = chapters_.back();
    last.end = duration_;
    if (last.start > last.end) {
        LOG(ERROR) << "Invalid last chapter!";
        exit(-1);
    }

    // get meta
    if (_GetMeta() != 0) { exit(-1); }

    // append to meta
    std::ofstream ofs(meta_file_, std::ios::out | std::ios::app);
    for (const auto &chapter : chapters_) {
        ofs << chapter.ToString() << std::endl;
    }
    ofs.close();

    // merge meta
    if (_MergeMeta() != 0) { exit(-1); }

    LOG(INFO) << "Done!";
}

int32_t GenChapter::_MergeMeta() {
    fs::path p(media_file_);
    fs::path ext = p.extension();
    p.replace_extension();
    std::string new_media_file = p.string();
    absl::StrAppend(&new_media_file, "_chapters", ext.string());

    std::string out;
    std::string cmd =
        absl::StrFormat("ffmpeg -i %s -i %s -map_metadata 1 -codec copy %s", media_file_, meta_file_, new_media_file);
    int32_t status = tools::run_cmd(cmd, &out);
    if (status != 0) {
        LOG(ERROR) << "Merge meta fail!";
        return -1;
    }
    return 0;
}

int32_t GenChapter::_GetMeta() {
    std::string out;
    std::string cmd =
        absl::StrFormat("ffmpeg -i %s -f ffmetadata %s", media_file_, meta_file_);
    int32_t status = tools::run_cmd(cmd, &out);
    if (status != 0) {
        LOG(ERROR) << "Get meta fail!";
        return -1;
    }
    return 0;
}

int32_t GenChapter::_GetDuration() {
    std::string out;
    std::string cmd =
        absl::StrFormat("mediainfo --Inform=\"General;%%Duration%%\" %s", media_file_);
    int32_t status = tools::run_cmd(cmd, &out);
    if (status != 0) {
        LOG(ERROR) << "Get duration fail!";
        return -1;
    }

    int32_t duration = 0;
    try {
        boost::trim(out);
        duration = boost::lexical_cast<int32_t>(out);
    } catch (const std::exception &ex) {
        LOG(ERROR) << "Get duration fail: " << ex.what();
        return -1;
    }

    return duration;
}

int32_t GenChapter::_Str2Time(const std::string &time) {
    std::vector<std::string> vec;
    boost::split(vec, time, boost::is_any_of(":"));
    if (vec.empty() || vec.size() > 3) { return -1; }
    std::reverse(vec.begin(), vec.end());

    int32_t timestamp = 0, base = 1;
    for (auto i = 0; i < vec.size(); ++i) {
        int32_t val = 0;
        try {
            val = boost::lexical_cast<int32_t>(vec[i]);
        } catch (...) {
            return -1;
        }
        timestamp += val * base;
        base *= 60;
    }

    return timestamp * 1000;
}

void GenChapter::_ReadChapter() {
    std::ifstream ifs(chapter_file_, std::ios::in);
    if (!ifs) {
        LOG(ERROR) << "Fail to read chapter file: " << chapter_file_;
        exit(-1);
    }

    int32_t line_cnt = 0;
    std::string line;
    while (std::getline(ifs, line)) {
        ++line_cnt;
        boost::trim(line);
        if (line.empty()) { continue; }
        std::istringstream iss(line);
        std::string time, title;
        iss >> time;
        int32_t timestamp = _Str2Time(time);
        if (timestamp < 0) {
            LOG(ERROR) << "Line " << line_cnt << ": Invalid time format " << time;
            exit(-1);
        }

        std::getline(iss, title);
        boost::trim(title);
        if (title.empty()) {
            LOG(ERROR) << "Line " << line_cnt << ": empty title!";
            exit(-1);
        }

        chapters_.emplace_back(title, timestamp);
    }
}

namespace {
bool check_args() {
    if (FLAGS_i.empty()) {
        LOG(ERROR) << "-i media file not specified!";
        return false;
    }
    if (FLAGS_c.empty()) {
        LOG(ERROR) << "-c chapter file not specified!";
        return false;
    }
    return true;
}
}  // namespace

int main(int argc, char **argv)
try {
    GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = true;
    google::InitGoogleLogging(argv[0]);

    if (!check_args()) { return -1; }

    GenChapter worker;
    worker.SetChapterFile(FLAGS_c);
    worker.SetMediaFile(FLAGS_i);
    worker.SetStartTitle(FLAGS_start);

    worker.Run();

    return 0;

} catch (const std::exception &ex) {
    std::cerr << "Exception caught by main: " << ex.what() << std::endl;
    return -1;
} catch (...) {
    std::cerr << "Unknown exception caught by main!" << std::endl;
    return -1;
}

