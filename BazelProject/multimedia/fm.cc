#include <cstdlib>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include <boost/filesystem.hpp>
#include "absl/strings/str_format.h"

#define MUSIC_FILE_TYPE "-iname \'*.flac\' -o -iname \'*.wav\' -o -iname \'*.ape\' -o -iname \'*.m4a\' -o -iname \'*.mp3\' -o -iname \'*.dff\' -o -iname \'*.dsf\'"

DEFINE_string(dir, "/music", "");
DEFINE_bool(a, false, "Album");
DEFINE_bool(d, false, "Album");

int main(int argc, char **argv) {
    namespace fs = boost::filesystem;

    int idx = gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = true;
    google::InitGoogleLogging(argv[0]);

    if (idx == argc) {
        LOG(ERROR) << "Search pattern cannot be empty!";
        return -1;
    }

    std::string pattern = argv[idx];
    if (pattern.empty()) {
        LOG(ERROR) << "Search pattern cannot be empty!";
        return -1;
    }

    if (!fs::exists(FLAGS_dir) || !fs::is_directory(FLAGS_dir)) {
        LOG(ERROR) << absl::StrFormat("%s is not a valid directory!", FLAGS_dir);
        return -1;
    }
    fs::current_path(FLAGS_dir);

    bool is_album = FLAGS_a || FLAGS_d;
    std::string cmd;

    if (is_album) {
        cmd = absl::StrFormat("find . -type d -iname \'*%s*\'", pattern);
        LOG(INFO) << "Running command: " << cmd;
        std::system(cmd.c_str());
        return 0;
    }

    // find single files
    cmd = absl::StrFormat("find . -type f \\( %s \\) | parallel --pipe grep -E \'%s\'", MUSIC_FILE_TYPE, pattern);
    LOG(INFO) << "Running command: " << cmd;
    std::system(cmd.c_str());

    // find in cue
    cmd = absl::StrFormat("find . -type f -iname \'*.cue\' | parallel \"grep -n -H -E \'%s\' {}\"", pattern);
    LOG(INFO) << "Running command: " << cmd;
    std::system(cmd.c_str());

    return 0;
}
