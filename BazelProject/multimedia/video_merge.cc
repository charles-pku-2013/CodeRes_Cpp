/*
bazel build -c opt //video_merge:video_merge
 */
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <initializer_list>
#include <iostream>
#include <boost/filesystem.hpp>
#include "absl/strings/ascii.h"
#include "absl/strings/str_join.h"
#include "absl/strings/match.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_split.h"
#include "absl/strings/strip.h"
#include <glog/logging.h>
#include <gflags/gflags.h>

DEFINE_string(d, ".", "target dir");
DEFINE_string(o, "output.mp4", "output file");
DEFINE_string(src_type, "", "File extension of video to be merged like \"mp4\", leave empty for auto detect");
DEFINE_string(mkv_tool, "/usr/local/bin/mkvmerge", "Tool that generate mkv file");  // TODO
DEFINE_string(mp4_tool, "/usr/local/bin/ffmpeg", "Tool that generate mp4 file");  // TODO
DEFINE_bool(keep_mkv, false, "Keep the intermidiate mkv file");
DEFINE_bool(n, false, "dry run, only print file list to be merged");
// DEFINE_bool(verbose, false, "verbose mode");

namespace fs = boost::filesystem;

class VideoMerge final {
 public:
    using FileTable = std::map<std::string, fs::path>;
    using FileTypeSet = std::unordered_set<std::string>;

 public:
    void SetTargetDir(const std::string &dir) { target_dir_ = dir; }

    void SetOutputFile(const std::string &filename) {
        mp4_file_ = filename;
        mkv_file_ = fs::change_extension(fs::path(filename), ".mkv").string();
    }

    void SetSrcType(const std::string &type)
    { src_type_ = "." + absl::AsciiStrToLower(type); }

    void SetMp4Tool(const std::string &tool) { mp4_tool_ = tool; }
    void SetMkvTool(const std::string &tool) { mkv_tool_ = tool; }
    void SetKeepMkv(bool val) { keep_mkv_ = val; }
    void SetDryRun(bool val) { dry_run_ = val; }
    void SetSupportedMediaTypes(std::initializer_list<std::string> l);

    void Run();

    std::string DebugString() const;

 private:
    static bool _RemoveFile(const std::string &file);

    void _Scan();
    void _GenMkv();
    void _GenMp4();

 private:
    std::string target_dir_;
    std::string src_type_;
    std::string mp4_file_, mkv_file_;
    std::string mp4_tool_, mkv_tool_;
    bool keep_mkv_ = false;
    FileTypeSet supported_media_types_;
    FileTable src_files_;
    bool dry_run_ = false;
};

void VideoMerge::SetSupportedMediaTypes(std::initializer_list<std::string> l) {
    FileTypeSet _types;
    for (const std::string &s : l) {
        _types.insert(absl::AsciiStrToLower(s));
    }
    supported_media_types_.swap(_types);
}

std::string VideoMerge::DebugString() const {
    return absl::StrFormat("target_dir:%s\nsrc_type:%s\nmp4_file:%s\nmkv_file:%s\n"
                           "mp4_tool:%s\nmkv_tool:%s\n",
                           target_dir_, src_type_, mp4_file_, mkv_file_, mp4_tool_, mkv_tool_);
}

bool VideoMerge::_RemoveFile(const std::string &file) {
    try {
        fs::remove(file);
    } catch (const std::exception &ex) {
        LOG(ERROR) << absl::StrFormat("Cannot remove file %s for %s", file, ex.what());
        return false;
    } catch (...) {
        LOG(ERROR) << absl::StrFormat("Cannot remove file %s for unknown reason", file);
        return false;
    }
    return true;
}

void VideoMerge::Run() {
    _Scan();
    if (src_files_.empty()) {
        std::cout << "No video to merge!" << std::endl;
        return;
    }

    std::cout << "The following videos will be merged into " << mp4_file_ << (dry_run_ ? " (DRY RUN)" : "") << std::endl;
    std::cout << absl::StrJoin(src_files_, "\n",
                [](std::string *out, const decltype(src_files_)::value_type &val){
        absl::StrAppend(out, val.first);
    });
    std::cout << std::endl;

    if (dry_run_) { return; }

    _GenMkv();
    _GenMp4();

    if (!keep_mkv_) {
        std::cout << "Removing " << mkv_file_ << "..." << std::endl;
        _RemoveFile(mkv_file_);
    }
}

void VideoMerge::_Scan() {
    std::string src_type = src_type_;
    FileTable src_files;

    for (fs::directory_iterator itr(target_dir_); itr != fs::directory_iterator(); ++itr) {
        if (!fs::is_regular_file(*itr)) { continue; }
        if (itr->path().filename().string() == mp4_file_ ||
                        itr->path().filename().string == mkv_file_)
        { continue; }

        std::string extension = absl::AsciiStrToLower(itr->path().extension().string());
        if (src_type.empty()) {
            // auto detect src type
            if (supported_media_types_.count(extension) > 0) {
                src_type = extension;
            } else {
                continue;   // skip unsupported files
            }
        } else {
            // src_type is explicitly specified or has been determined by auto detecting
            if (extension != src_type) { continue; }  // skip unsupported
        }

        src_files[itr->path().filename().string()] = itr->path();
    }

    src_files_.swap(src_files);
}

void VideoMerge::_GenMkv() {
    if (!_RemoveFile(mkv_file_)) {
        throw std::runtime_error(absl::StrFormat("%s already exists and cannot be removed!", mkv_file_));
    }
}

void VideoMerge::_GenMp4() {
    if (!_RemoveFile(mp4_file_)) {
        throw std::runtime_error(absl::StrFormat("%s already exists and cannot be removed!", mp4_file_));
    }
}

int main(int argc, char **argv)
try {
    GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = true;
    google::InitGoogleLogging(argv[0]);

    if (!fs::exists(FLAGS_d) || !fs::is_directory(FLAGS_d)) {
        LOG(ERROR) << absl::StrFormat("Specified dir %s is not a valid directory!", FLAGS_d);
        return -1;
    }

    VideoMerge vm;
    vm.SetSupportedMediaTypes({".mp4", ".flv", ".avi", ".ts", ".mkv"});
    vm.SetTargetDir(FLAGS_d);
    vm.SetSrcType(FLAGS_src_type);
    vm.SetOutputFile(FLAGS_o);
    vm.SetMp4Tool(FLAGS_mp4_tool);
    vm.SetMkvTool(FLAGS_mkv_tool);
    vm.SetKeepMkv(FLAGS_keep_mkv);
    vm.SetDryRun(FLAGS_n);

    std::cout << "VideoMerge:\n" << vm.DebugString() << std::endl;

    vm.Run();

    return 0;

} catch (const std::exception &ex) {
    std::cerr << argv[0] << " terminated by exception: " << ex.what() << std::endl;
    return -1;
} catch (...) {
    std::cerr << argv[0] << " terminated by unknown exception!" << std::endl;
    return -1;
}

