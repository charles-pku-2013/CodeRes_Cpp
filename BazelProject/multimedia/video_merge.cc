/*
bazel build -c opt //video_merge:video_merge
 */
#include <cstdlib>
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
DEFINE_string(mkv, "mkvmerge", "tool for generating mkv");
DEFINE_string(mp4, "ffmpeg", "tool for generating mp4");
DEFINE_bool(no_mp4, false, "do not create mp4");
DEFINE_bool(no_mkv, true, "remove mkv if create mp4");

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

    // mkv command
    {
        try { fs::remove(FLAGS_o + ".mkv"); } catch (...) {}  // remove if exists
        auto it = src_files.begin();
        std::string cmd_mkv = FLAGS_mkv + " -o " + FLAGS_o + ".mkv " + *it;
        ++it;
        for (; it != src_files.end(); ++it) {
            cmd_mkv.append(" +").append(*it);
        }
        LOG(INFO) << "cmd_mkv: " << cmd_mkv;
        ::system(cmd_mkv.c_str());
    }

    // ffmpeg command
    if (!FLAGS_no_mp4)
    {
        if (!fs::exists(FLAGS_o + ".mkv")) {
            LOG(ERROR) << "Cannot create mp4 for mkv file does not exist!";
            return 0;
        }
        try { fs::remove(FLAGS_o + ".mp4"); } catch (...) {}  // remove if exists
        std::string cmd_mp4 = absl::StrFormat("%s -i %s -filter_complex "
                "\"[0:v:0][0:a:0]concat=n=1:v=1:a=1[outv][outa]\" -map \"[outv]\" -map \"[outa]\" "
                "-preset superfast -profile:v high %s", FLAGS_mp4, (FLAGS_o + ".mkv"), (FLAGS_o + ".mp4"));
        LOG(INFO) << "cmd_mp4: " << cmd_mp4;
        int retval = ::system(cmd_mp4.c_str());
        if (0 == retval && FLAGS_no_mkv) {
            try { fs::remove(FLAGS_o + ".mkv"); } catch (...) {}
        }
    }

    return 0;

} catch (const std::exception &ex) {
    std::cerr << argv[0] << " terminated by exception: " << ex.what() << std::endl;
    return -1;
} catch (...) {
    std::cerr << argv[0] << " terminated by unknown exception!" << std::endl;
    return -1;
}

