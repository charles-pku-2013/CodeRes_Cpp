#include <google/protobuf/message.h>
#include <sys/time.h>

template<typename ProtoType>
bool save_pb(const ProtoType *msg, const std::string &fname) {
    std::ofstream ofs(fname, std::ios::out | std::ios::binary);
    if (!ofs) { return false; }
    return msg->SerializeToOstream(&ofs);
}

template<typename ProtoType>
bool load_pb(const std::string &fname, ProtoType *msg) {
    std::ifstream ifs(fname, std::ios::in | std::ios::binary)
    if (!ifs) { return false; }
    return msg->ParseFromIstream(&ifs);
}

// 微秒级时间戳
bool ClassName::_SaveErrReq(const RequestNew &req, const std::string &err_msg) {
    if (!save_err_req_) { return false; }

    try {
        char buf[64] = {'\0'};
        struct timeval tv;
        gettimeofday(&tv, NULL);
        std::tm tm;
        localtime_r(&tv.tv_sec, &tm);
        std::size_t n = std::strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tm);
        snprintf(buf + n, sizeof(buf) - n, "_%06ld", tv.tv_usec);

        // save binary req data
        if (!save_pb(&req, err_req_dir_ + "/" + buf + ".dat")) {
            LOG(ERROR) << DebugString() << " save request fail!";
            return false;
        }

        // save err msg
        std::ofstream ofs(err_req_dir_ + "/" + buf + ".txt");
        if (!ofs) {
            LOG(ERROR) << DebugString() << " save error msg fail!";
            return false;
        }
        ofs << err_msg << std::endl;

        return true;
    } catch (const std::exception &ex) {
        LOG(ERROR) << DebugString() << " _SaveErrReq error: " << ex.what();
        return false;
    } catch (...) {
        LOG(ERROR) << DebugString() << " _SaveErrReq unknown error!";
        return false;
    }
}
