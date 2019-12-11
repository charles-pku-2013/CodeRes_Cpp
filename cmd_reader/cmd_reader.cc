#include "rtmp_server/common/cmd_reader.h"
#include <glog/logging.h>
#include <boost/algorithm/string.hpp>
#include <streambuf>
#include <iterator>

namespace common {

std::unique_ptr<ReadCmdResult> CmdReader::run(const std::string &cmd) {
    std::string cmd_str = "stdbuf -oL ";
    cmd_str.append(cmd).append(" 2>&1");

    std::unique_ptr<ReadCmdResult> ret(new ReadCmdResult);
    ret->fp = ::popen(cmd_str.c_str(), "r");
    if (!ret->fp) {
        LOG(ERROR) << "Run cmd " << cmd << " fail!";
        return nullptr;
    }
    ::setlinebuf(ret->fp);
    ret->p_stream.reset(new ReadCmdResult::FdStream(::fileno(ret->fp), boost::iostreams::never_close_handle));

    return ret;
}

int CmdReader::run(const std::string &cmd, std::string *output) {
    common::CmdReader cmd_reader;
    auto ret = cmd_reader.run(cmd);
    std::istreambuf_iterator<char> beg(ret->stream()), end;
    output->assign(beg, end);
    boost::trim(*output);
    return ret->finish();
}

}  // namespace common

