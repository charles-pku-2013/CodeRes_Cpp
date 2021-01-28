#include <cstdlib>
#include <string>
#include <unordered_map>
#include <unordered_set>
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

    using RawFileSet = std::unordered_map<std::string, std::string>;
    RawFileSet raw_file_set;
    std::unordered_set<std::string> jpg_file_set;
    for (fs::directory_iterator itr("."); itr != fs::directory_iterator(); ++itr) {
        if (!fs::is_regular_file(*itr)) { continue; }
        if (absl::EqualsIgnoreCase(itr->path().extension().string(), "." + FLAGS_raw)) {
            raw_file_set[itr->path().stem().string()] = itr->path().filename().string();
        } else if (absl::EqualsIgnoreCase(itr->path().extension().string(), ".jpg") ||
                absl::EqualsIgnoreCase(itr->path().extension().string(), ".jpeg")) {
            jpg_file_set.emplace(itr->path().stem().string());
        }
    }  // for itr

    for (const auto& item : jpg_file_set) {
        raw_file_set.erase(item);
    }

    if (raw_file_set.empty()) {
        LOG(INFO) << "No file found";
        return 0;
    }

    auto remove_files = [&] {
        for (const auto& kv : raw_file_set) {
            try {
                LOG(INFO) << "Removing " << kv.second;
                fs::remove(kv.second);
            } catch (const std::exception &ex) {
                LOG(ERROR) << "Fail to remove file " << kv.second << ". " << ex.what();
            }
        }
    };

    if (FLAGS_y) {
        remove_files();
    } else {
        std::cerr << "The following files will be removed:\n"
            << absl::StrJoin(raw_file_set, "\n", [](std::string *out, const RawFileSet::value_type &val){
                    absl::StrAppend(out, val.second); })
            << "\nPlease confirm (Y/n): " << std::flush;
        std::string input;
        std::getline(std::cin, input);
        absl::StripAsciiWhitespace(&input);
        if (absl::EqualsIgnoreCase(input, "y")) {
            remove_files();
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

