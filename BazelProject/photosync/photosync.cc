#include <cstdlib>
#include <string>
#include <map>
#include <iostream>
#include <boost/filesystem.hpp>
#include "absl/strings/str_join.h"
#include "absl/strings/match.h"
#include "absl/strings/str_format.h"
#include <glog/logging.h>
#include <gflags/gflags.h>

DEFINE_string(src_dir, ".", "source dir");
DEFINE_string(dst_dir, ".", "target dir");
DEFINE_string(src_type, "", "source file type like jpg");
DEFINE_string(dst_type, "", "target file type like raf");
DEFINE_string(exclude, "", "file or dir to ignore");
DEFINE_bool(n, false, "dry run");
DEFINE_bool(verbose, false, "verbose mode");

namespace fs = boost::filesystem;

class PhotoSync final {
 public:
    using FileTable = std::map<std::string, fs::path>;

 public:
    PhotoSync(const std::string &src_path, const std::string dst_path,
            const std::string &src_type, const std::string &dst_type)
                : src_path_(src_path), dst_path_(dst_path),
                  src_type_(src_type), dst_type_(dst_type) {}

    void Run(bool dry_run);

    void SetVerbose(bool val) { verbose_ = val; }
    void SetExclude(const std::string& exclude) { exclude_ = exclude; }

    std::string DebugString() const;

 private:
    static void _ScanPath(const std::string &path, const std::string &file_type, const std::string &exclude,
                          FileTable *_file_table);

 private:
    std::string src_path_, dst_path_, src_type_, dst_type_, exclude_;
    FileTable src_files_, dst_files_, remove_files_;
    bool verbose_ = false;
};

void PhotoSync::Run(bool dry_run) {
    src_files_.clear();
    dst_files_.clear();
    remove_files_.clear();

    _ScanPath(src_path_, src_type_, exclude_, &src_files_);
    _ScanPath(dst_path_, dst_type_, exclude_, &dst_files_);

    for (auto& kv : dst_files_) {
        if (src_files_.count(kv.first) == 0) {
            remove_files_[kv.first] = kv.second;
        }
    }

    if (verbose_) {
        std::cout << "PhotoSync: {" << DebugString() << "}" << std::endl;
    }

    if (remove_files_.empty()) {
        std::cout << "No file to remove" << std::endl;
        return;
    }

    std::cout << "The following files will be removed: " << (dry_run ? "(dry run)" : "") << std::endl;
    for (auto& kv : remove_files_) {
        std::cout << kv.second.string() << std::endl;
    }
    std::cout << std::endl;

    // remove files
    if (!dry_run) {
        for (auto& kv : remove_files_) {
            try {
                std::cout << "Removing " << kv.second << std::endl;
                fs::remove(kv.second);
            } catch (const std::exception &ex) {
                LOG(ERROR) << "Fail to remove file " << kv.second << ". " << ex.what();
            }
        }
    }  // if !dry_run
}

void PhotoSync::_ScanPath(const std::string &path, const std::string &file_type, const std::string &exclude,
                          FileTable *_file_table) {
    FileTable file_table;
    for (fs::recursive_directory_iterator itr(path); itr != fs::recursive_directory_iterator(); ++itr) {
        if (!fs::is_regular_file(*itr)) { continue; }
        // 路径中包含排除字段
        if (!exclude.empty() && itr->path().string().find(exclude) != std::string::npos) { continue; }
        if (absl::EqualsIgnoreCase(itr->path().extension().string(), "." + file_type)) {
            // key like ./DSC_1888 /photo/xxx/DSC_1888
            // fs::path key = fs::relative(itr->path().parent_path(), path) / itr->path().stem();
            // file_table[key.string()] = itr->path();
            // key is just stem like DSC_1888
            file_table[itr->path().stem().string()] = itr->path();
        }
    }
    _file_table->swap(file_table);
}

std::string PhotoSync::DebugString() const {
    std::string str_src_files = absl::StrJoin(src_files_, ",",
            [](std::string *out, const decltype(src_files_)::value_type &val){
        absl::StrAppend(out, "{", val.first, ":", val.second.string(), "}");
    });
    std::string str_dst_files = absl::StrJoin(dst_files_, ",",
            [](std::string *out, const decltype(dst_files_)::value_type &val){
        absl::StrAppend(out, "{", val.first, ":", val.second.string(), "}");
    });
    std::string str_remove_files = absl::StrJoin(remove_files_, ",",
            [](std::string *out, const decltype(remove_files_)::value_type &val){
        absl::StrAppend(out, "{", val.first, ":", val.second.string(), "}");
    });
    return absl::StrFormat("src_path:%s, dst_path:%s, src_type:%s, dst_type:%s, "
                           "src_files:{%s}, dst_files:{%s}, remove_files:{%s}",
                           src_path_, dst_path_, src_type_, dst_type_, str_src_files, str_dst_files, str_remove_files);
}

namespace {
bool check_args() {
    if (FLAGS_src_type.empty()) {
        LOG(ERROR) << "-src_type not specified!";
        return false;
    }
    if (FLAGS_dst_type.empty()) {
        LOG(ERROR) << "-dst_type not specified!";
        return false;
    }
    if (!fs::exists(FLAGS_src_dir) || !fs::is_directory(FLAGS_src_dir)) {
        LOG(ERROR) << "src_dir: " << FLAGS_src_dir << " is invalid!";
        return false;
    }
    if (!fs::exists(FLAGS_dst_dir) || !fs::is_directory(FLAGS_dst_dir)) {
        LOG(ERROR) << "dst_dir: " << FLAGS_dst_dir << " is invalid!";
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

    PhotoSync ps(FLAGS_src_dir, FLAGS_dst_dir, FLAGS_src_type, FLAGS_dst_type);
    ps.SetVerbose(FLAGS_verbose);
    ps.SetExclude(FLAGS_exclude);
    ps.Run(FLAGS_n);

    return 0;

} catch (const std::exception &ex) {
    std::cerr << argv[0] << " terminated by exception: " << ex.what() << std::endl;
    return -1;
} catch (...) {
    std::cerr << argv[0] << " terminated by unknown exception!" << std::endl;
    return -1;
}


