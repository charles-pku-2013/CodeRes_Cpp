/*
bazel build -c opt //hls_cleanup:hls_cleanup
 */
#include <string>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "absl/strings/match.h"
#include <glog/logging.h>

namespace fs = boost::filesystem;

bool get_serial_no(const std::string& str, int64_t *no) {
    std::string main_name = fs::path(str).stem().string();
    std::string::size_type pos = main_name.rfind('-');
    if (pos == std::string::npos) { return false; }
    std::string str_serial = main_name.substr(pos + 1);
    if (str_serial.empty()) { return false; }
    try {
        int64_t value = boost::lexical_cast<int64_t>(str_serial);
        *no = value;
        return true;
    } catch (...) {
        return false;
    }
    return false;
}

int main(int argc, char **argv) {
    FLAGS_logtostderr = true;
    google::InitGoogleLogging(argv[0]);

    if (argc < 2) {
        LOG(ERROR) << "Usage: " << argv[0] << " m3u8_file";
        return 0;
    }

    fs::path m3u8_path(argv[1]);
    if (!fs::exists(m3u8_path) || !fs::is_regular_file(m3u8_path)) {
        LOG(ERROR) << "hls_cleanup " << m3u8_path << "is not valid!";
        return 0;
    }

    std::string m3u8_bak = "/tmp/" + m3u8_path.filename().string();
    try { fs::remove(m3u8_bak); } catch (...) {}  // remove if exists
    try {
        fs::copy_file(m3u8_path, m3u8_bak);
    } catch (const std::exception &ex) {
        LOG(ERROR) << "hls_cleanup fail to create copy of " << m3u8_path << " " << ex.what();
        return 0;
    }
    std::unique_ptr<void, std::function<void(void*)>> _on_finish((void*)1, [&m3u8_bak](void*){
        try { fs::remove(m3u8_bak); } catch (...) {}
    });

    std::string stream_name = m3u8_path.stem().string();

    std::ifstream ifs(m3u8_bak, std::ios::in);
    if (!ifs) {
        LOG(ERROR) << "hls_cleanup cannot open " << m3u8_bak << " for reading!";
        return 0;
    }

    std::string line;
    int64_t base_no = -1;
    while (std::getline(ifs, line)) {
        boost::trim(line);
        if (absl::EndsWith(line, ".ts") && absl::StartsWith(line, stream_name + "-")) {
            if (get_serial_no(line, &base_no))
            { break; }
        }
    }
    ifs.close();

    LOG(INFO) << "hls_cleanup base_no = " << base_no;
    if (base_no <= 0) { return 0; }

    fs::path stream_path = m3u8_path.parent_path();
    for (fs::directory_iterator itr(stream_path); itr != fs::directory_iterator(); ++itr) {
        fs::path fpath = itr->path();
        std::string fname = fpath.filename().string();
        if (!absl::EndsWith(line, ".ts") || !absl::StartsWith(line, stream_name + "-")) {
            continue;
        }
        int64_t no = -1;
        if (!get_serial_no(fname, &no)) { continue; }
        if (no < base_no && no >= 0) {
            try {
                LOG(INFO) << "hls_cleanup removing " << fpath;
                fs::remove(fpath);
            } catch (const std::exception &ex) {
                LOG(ERROR) << "hls_cleanup cannot delete file " << fpath << ". " << ex.what();
            }
        }  // if
    }  // for

    return 0;
}

