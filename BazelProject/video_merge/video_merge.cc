/*
bazel build -c opt //video_merge:video_merge
 */
#include <string>
#include <set>
#include <iostream>
#include <boost/filesystem.hpp>
#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_format.h"
#include "absl/strings/strip.h"
#include <glog/logging.h>
#include <gflags/gflags.h>

DEFINE_string(d, ".", "target dir");
DEFINE_string(src_type, "flv", "source file extension");
DEFINE_string(o, "", "output file main name");

namespace fs = boost::filesystem;

namespace {
bool _check_output = gflags::RegisterFlagValidator(&FLAGS_o,
            [](const char* flagname, const std::string& value){
    if (value.empty()) {
        std::cerr << flagname << " not set" << std::endl;
        return false;
    }
    return true;
});
}  // namespace

int main(int argc, char **argv)
try {
    GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = true;
    google::InitGoogleLogging(argv[0]);

    if (!fs::exists(FLAGS_d) || !fs::is_directory(FLAGS_d)) {
        LOG(ERROR) << "Invalid target directory!";
        return -1;
    }

    fs::current_path(FLAGS_d);

    std::set<std::string> src_files;
    for (fs::directory_iterator itr("."); itr != fs::directory_iterator(); ++itr) {
        if (fs::is_regular_file(*itr) && itr->path().extension() == "." + FLAGS_src_type) {
            src_files.insert(static_cast<std::string>(absl::StripPrefix(itr->path().string(), "./")));
        }
    }
    LOG(INFO) << absl::StrFormat("Merging sequence: {%s}", absl::StrJoin(src_files, ", "));

    if (src_files.empty()) { return 0; }

    return 0;

} catch (const std::exception &ex) {
    std::cerr << argv[0] << " terminated by exception: " << ex.what() << std::endl;
    return -1;
} catch (...) {
    std::cerr << argv[0] << " terminated by unknown exception!" << std::endl;
    return -1;
}

