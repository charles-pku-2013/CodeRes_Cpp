#pragma once
#include <string>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>

namespace common {

struct ReadCmdResult final {
    using FdStream = boost::iostreams::stream<boost::iostreams::file_descriptor_source>;

    ~ReadCmdResult() { finish(); }

    FdStream& stream() { return *p_stream; }

    int finish() {
        if (!done) {
            status_code = ::pclose(fp);
            status_code = WEXITSTATUS(status_code);
            done = true;
        }
        return status_code;
    }

    int status() const { return status_code; }

    std::unique_ptr<FdStream> p_stream;
    FILE *fp = nullptr;  // by popen pclose
    int status_code = 0;
    bool done = false;
};

struct CmdReader final {
    std::unique_ptr<ReadCmdResult> run(const std::string &cmd);
    int run(const std::string &cmd, std::string *output);
};

}  // namespace common

