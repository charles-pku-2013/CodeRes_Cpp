#include "run_cmd.h"
#include <cstdio>
#include <sstream>
#include <memory>
#include <sys/types.h>
#include <sys/wait.h>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>

namespace tools {

int run_cmd(const std::string &cmd, std::string *out) {
    int retval = -1;
    std::unique_ptr<FILE, std::function<void(FILE*)>> pp(
        ::popen(cmd.c_str(), "r"),
        [&retval](FILE* p) {
            if(p) {
                retval = ::pclose(p);
                retval = WEXITSTATUS(retval);
            }
        }
    );
    if (!pp) {
        *out = "popen error!";
        return -1;
    }
    ::setlinebuf(pp.get());
    // ::setvbuf(pp.get(), NULL, _IOLBF, 0);

    using PipeStream = boost::iostreams::stream<boost::iostreams::file_descriptor_source>;
    PipeStream stream(fileno(pp.get()), boost::iostreams::never_close_handle);

    std::stringstream ss;
    ss << stream.rdbuf();

    *out = ss.str();

    pp.reset();   // must call it manually
    return retval;
}

}  // namespace tools

