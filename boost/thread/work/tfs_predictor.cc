#include "kernels/algo/adapters/predictor/tfs_predictor.h"
#include <boost/filesystem.hpp>
#include <glog/logging.h>
#include <fstream>
#include <thread>
#include <google/protobuf/message.h>
#include "common/observer/monitor.h"

#define SLEEP_MILLISECONDS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))


namespace {
template<typename ProtoType>
bool save_req(ProtoType *msg) {
    static std::atomic<uint32_t> cnt(0);
    static constexpr char path[] = "/export/merger_tfs_req/";
    uint32_t no = ++cnt;
    char fname[128];
    snprintf(fname, 128, "%s%05u.req", path, no);
    std::ofstream ofs(fname, std::ios::out | std::ios::binary);
    if (!ofs) { return false; }
    return msg->SerializeToOstream(&ofs);
}
}  // namespace

bool TFSPredictor::Init(const std::string& base_path) {
    namespace fs = boost::filesystem;
    fs::path _base_path(base_path);

    if (!fs::is_directory(_base_path)) {
        LOG(ERROR) << "GBDTPredictor specified base path " << base_path << " is not a directory!";
        return false;
    }

    // must load feature meta first
    fs::path feature_meta_file = _base_path / "feature.meta";
    if (!_LoadFeatureInfo(feature_meta_file.string())) {
        LOG(ERROR) << DebugString() << " init fail!";
        return false;
    }

    // 这种方法正确请求的错误率和优化之前的是一样的
    // static constexpr char TFS_SERVER[] = "jsf://com.jd.search.TfsProxy:tfsProxy";
    // static constexpr char LOAD_BALANCER[] = "rr";
    // static constexpr int CLIENT_POOL_SIZE = 6;  // TODO(sunchao)

    // for (int i = 0; i < CLIENT_POOL_SIZE; ++i) {
        // if (!client_pool_.Add(TFS_SERVER, LOAD_BALANCER)) {
            // LOG(ERROR) << "Failed to init tfs client to " << TFS_SERVER;
            // continue;
        // }
    // }
    
    // 这种方法正确请求的成功率大大提高，但也有错误
    client_pool_.Add("10.189.4.7:8000", "");
    client_pool_.Add("10.190.254.174:8000", "");
    client_pool_.Add("10.181.200.174:8000", "");
    client_pool_.Add("10.190.254.171:8000", "");
    client_pool_.Add("10.180.128.7:8000", "");
    client_pool_.Add("10.181.60.193:8000", "");

    if (0 == client_pool_.Size()) {
        LOG(ERROR) << "No tfs server available!";
        return false;
    }

    LOG(INFO) << "TFSPredictor init success. client pool size " << client_pool_.Size();

    return true;
}

bool TFSPredictor::Predict(const Request &req, Response *res, int timeout,
                            std::string *err_msg) {
    static constexpr int TRY_CNT = 1;  // TODO(sunchao)
    for (int i = 0; i < TRY_CNT; ++i) {
        int status = client_pool_.GetData(req, res, timeout, err_msg);
        if (status == TfsClient::SUCCESS) {
            Observer::gMonitor()->trace("tps.available", 1, Observer::T_RAT);
            return true;
        }
        if (status == TfsClient::FAIL) {
            Observer::gMonitor()->trace("tps.available", 0, Observer::T_RAT);
            LOG(ERROR) << "TFSPredictor predict fail!" << (err_msg ? " " + *err_msg : "");
            continue;
        }
        if (status == TfsClient::BUSY) {
            LOG(WARNING) << "Temporary no active tfs server available, try later.";
            SLEEP_MILLISECONDS(10);
        }
    }
    return false;
}

REGISTER_PREDICTOR_FACTORY("tfs", TFSPredictor);

