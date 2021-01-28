#include <cstdlib>
#include <string>
#include <set>
#include <iostream>
#include <boost/filesystem.hpp>
#include "absl/strings/str_join.h"
#include "absl/strings/strip.h"
#include "absl/strings/match.h"
#include <glog/logging.h>
#include <gflags/gflags.h>

DEFINE_string(dir, ".", "target dir");
DEFINE_string(raw, "raf", "raw file type");
DEFINE_bool(y, false, "remove without confirm");


namespace fs = boost::filesystem;

template<typename Iter>
void remove_files(Iter beg, Iter end) {
    for(; beg != end; ++beg) {
        try {
            LOG(INFO) << "Removing " << *beg;
            fs::remove(*beg);
        } catch (const std::exception &ex) {
            LOG(ERROR) << "Fail to remove file " << *beg << ". " << ex.what();
        }
    }
}

int main(int argc, char **argv)
try {
    GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = true;
    google::InitGoogleLogging(argv[0]);

    if (!fs::exists(FLAGS_dir) || !fs::is_directory(FLAGS_dir)) {
        LOG(ERROR) << "Invalid dir " << FLAGS_dir;
        return -1;
    }

    fs::current_path(FLAGS_dir);

    std::set<std::string> to_be_removed;
    for (fs::directory_iterator itr("."); itr != fs::directory_iterator(); ++itr) {
        if (!fs::is_regular_file(*itr)) { continue; }
        if (absl::EqualsIgnoreCase(itr->path().extension().string(), FLAGS_raw)) {
            std::string jpg_file1 = itr->path().stem().string() + ".jpg";
            std::string jpg_file2 = itr->path().stem().string() + ".JPG";
            if (!fs::exists(jpg_file1) && !fs::exists(jpg_file2)) {
                to_be_removed.insert(itr->path().filename().string());
            }
        }
    }  // for itr

    if (to_be_removed.empty()) {
        LOG(INFO) << "No file found";
        return 0;
    }

    if (FLAGS_y) {
        remove_files(to_be_removed.begin(), to_be_removed.end());
    } else {
        std::cerr << "The following files will be removed:\n"
            << absl::StrJoin(to_be_removed, "\n")
            << "Please confirm (Y/n): " << std::flush;
        std::string input;
        std::getline(std::cin, input);
        absl::StripAsciiWhitespace(&input);
        if (absl::EqualsIgnoreCase(input, "y")) {
            remove_files(to_be_removed.begin(), to_be_removed.end());
        } else {
            LOG(INFO) << "Aborted!";
            return 0;
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

