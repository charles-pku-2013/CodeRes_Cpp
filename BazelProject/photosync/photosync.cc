/*
bazel build -c opt //photosync:photosync
 */
#include <cstdlib>
#include <string>
#include <map>
#include <unordered_set>
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

DEFINE_string(src_dir, ".", "source dir");
DEFINE_string(dst_dir, ".", "target dir");
DEFINE_string(src_type, "", "source file type like jpg");
DEFINE_string(dst_type, "", "target file types like raf,nef Use \',\' to seperate multiple items");
DEFINE_string(task, "remove", "task type \"move\" or \"remove\"");
DEFINE_string(exclude, "", "file or dir to ignore");
DEFINE_bool(n, false, "dry run");
DEFINE_bool(verbose, false, "verbose mode");

namespace fs = boost::filesystem;

class PhotoSync final {
 public:
    using FileTable = std::map<std::string, fs::path>;
    using FileTypeSet = std::unordered_set<std::string>;

    enum _TaskType : int32_t {
        REMOVE, MOVE,
    };

 public:
    PhotoSync(const std::string &src_path, const std::string &dst_path,
              const std::string &src_type, const std::string &dst_type);

    void Run();

    void SetVerbose(bool val) { verbose_ = val; }
    void SetDryRun(bool val) { dry_run_ = val; }
    void SetExclude(const std::string& exclude) { exclude_ = exclude; }
    void SetTaskType(const std::string& name);

    std::string DebugString() const;

 private:
    template<typename Iter>
    void _SetDstTypes(Iter beg, Iter end) {
        FileTypeSet types(beg, end);
        dst_types_.swap(types);
    }

    void _RunRemove();
    void _RunMove();

    static std::string _TaskType2String(int32_t task_type);
    static void _ScanPath(const std::string &path, const FileTypeSet &file_types, const std::string &exclude,
                          FileTable *_file_table);

 private:
    std::string src_path_, dst_path_, src_type_, exclude_;
    FileTypeSet dst_types_;
    FileTable src_files_, dst_files_;
    bool verbose_ = false, dry_run_ = false;
    int32_t task_ = REMOVE;
};

PhotoSync::PhotoSync(const std::string &src_path, const std::string &dst_path,
                     const std::string &src_type, const std::string &dst_type)
                            : src_path_(src_path), dst_path_(dst_path) {
    src_type_ = "." + absl::AsciiStrToLower(src_type);
    std::vector<std::string> dst_types = absl::StrSplit(dst_type,
            absl::ByAnyChar(","), absl::SkipWhitespace()); // 这里默认返回类型是string_view所以不可以用auto
    for (auto& item : dst_types) {
        absl::StripAsciiWhitespace(&item);
        item = "." + absl::AsciiStrToLower(item);
    }
    _SetDstTypes(dst_types.begin(), dst_types.end());
    if (dst_types_.empty()) {
        throw std::runtime_error("PhotoSync dst_types is empty!");
    }
    if (dst_types_.count(src_type_) > 0) {
        throw std::runtime_error(absl::StrFormat("PhotoSync src_type \"%s\" in dst_type set", src_type_));
    }
}

void PhotoSync::SetTaskType(const std::string& name) {
    if (name == "move") {
        task_ = MOVE;
    } else if (name == "remove") {
        task_ = REMOVE;
    } else {
        throw std::runtime_error(absl::StrFormat("Unknown task \"%s\"", name));
    }
}

std::string PhotoSync::_TaskType2String(int32_t task_type) {
    switch(task_type) {
        case REMOVE: { return "remove"; }
        case MOVE: { return "move"; }
    }
    return "invalid";
}

void PhotoSync::Run() {
    src_files_.clear();
    dst_files_.clear();

    FileTypeSet src_types{src_type_};
    _ScanPath(src_path_, src_types, exclude_, &src_files_);
    _ScanPath(dst_path_, dst_types_, exclude_, &dst_files_);

    switch (task_) {
        case REMOVE:
            _RunRemove();
            break;
        case MOVE:
            _RunMove();
            break;
    }
}

void PhotoSync::_RunRemove() {
    FileTable remove_files;

    for (auto& kv : dst_files_) {
        if (src_files_.count(kv.first) == 0) {
            remove_files[kv.first] = kv.second;
        }
    }

    if (verbose_) {
        std::cout << "PhotoSync: {" << DebugString() << "}" << std::endl;
    }

    if (remove_files.empty()) {
        std::cout << "No file to remove" << std::endl;
        return;
    }

    std::cout << "The following files will be removed: " << (dry_run_ ? "(DRY RUN)" : "") << std::endl;
    for (auto& kv : remove_files) {
        std::cout << kv.second.string() << std::endl;
    }
    std::cout << std::endl;

    // remove files
    if (!dry_run_) {
        for (auto& kv : remove_files) {
            try {
                std::cout << "Removing " << kv.second << std::endl;
                fs::remove(kv.second);
            } catch (const std::exception &ex) {
                LOG(ERROR) << "Fail to remove file " << kv.second << ". " << ex.what();
            }
        }
    }  // if !dry_run_
}

void PhotoSync::_RunMove() {
    throw std::runtime_error("Move not implemented!");
}

void PhotoSync::_ScanPath(const std::string &path, const FileTypeSet &file_types, const std::string &exclude,
                          FileTable *_file_table) {
    FileTable file_table;
    for (fs::recursive_directory_iterator itr(path); itr != fs::recursive_directory_iterator(); ++itr) {
        if (!fs::is_regular_file(*itr)) { continue; }
        // 路径中包含排除字段
        if (!exclude.empty() && itr->path().string().find(exclude) != std::string::npos) { continue; }
        std::string extension = absl::AsciiStrToLower(itr->path().extension().string());
        if (file_types.count(extension) > 0) {
            // if (absl::EqualsIgnoreCase(itr->path().extension().string(), "." + file_type))
            // key like ./DSC_1888 /photo/xxx/DSC_1888
            // fs::path key = fs::relative(itr->path().parent_path(), path) / itr->path().stem();
            // file_table[key.string()] = itr->path();
            // key is just stem like DSC_1888
            // file_table[itr->path().stem().string()] = itr->path();
            auto ret = file_table.emplace(itr->path().stem().string(), itr->path());
            if (!ret.second) {
                LOG(ERROR) << absl::StrFormat("Failed to insert file item {%s:%s} "
                        "for same item with path %s already exists!",
                        itr->path().stem().string(), itr->path().string(), ret.first->second.string());
            }
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
    return absl::StrFormat("src_path:%s, dst_path:%s, src_type:%s, dst_type:{%s}, task:%s, "
                           "src_files:{%s}, dst_files:{%s}",
                           src_path_, dst_path_, src_type_,
                           absl::StrJoin(dst_types_, ","), _TaskType2String(task_),
                           str_src_files, str_dst_files);
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
    ps.SetDryRun(FLAGS_n);
    ps.SetTaskType(FLAGS_task);
    ps.Run();

    return 0;

} catch (const std::exception &ex) {
    std::cerr << argv[0] << " terminated by exception: " << ex.what() << std::endl;
    return -1;
} catch (...) {
    std::cerr << argv[0] << " terminated by unknown exception!" << std::endl;
    return -1;
}


