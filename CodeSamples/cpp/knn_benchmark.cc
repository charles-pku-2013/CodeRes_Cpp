/*
# build:
bazel build -c opt //tensorflow_serving/example:knn_benchmark
# usage
knn_benchmark -d req_data_dir -thread_num 10 -server ip:port
 */
#include <unistd.h>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <boost/filesystem.hpp>
#include "absl/strings/str_format.h"
#include "rate_limiter/rate_limiter.h"
#include "rate_limiter/rate_limiter_factory.h"

// bazel-genfiles/tensorflow_serving/apis
#include "tensorflow_serving/apis/knnpred_service.grpc.pb.h"
#include "tensorflow_serving/apis/knnpred_service.pb.h"

#include <grpcpp/grpcpp.h>

DEFINE_string(server, "", "server addr ip:port");
DEFINE_string(d, ".", "directory containing request data");
DEFINE_int32(thread_num, 1, "number of thread");
DEFINE_int32(qps, 0, "QPS limit");
DEFINE_int32(r, 1, "statistic rate every N seconds");
DEFINE_int32(t, 0, "run -t rounds of test");
DEFINE_string(model_name, "", "reset model name of request");
DEFINE_int64(model_ver, 0, "reset model version of requeest");

namespace fs = boost::filesystem;

using KnnRequest = tensorflow::serving::KnnPredSearchRequest;
using KnnResponse = tensorflow::serving::KnnPredSearchResult;
using KnnService = tensorflow::serving::KnnPredService;

template<typename ServiceType>
class RpcClient {
 public:
    using StubType = typename ServiceType::Stub;

 public:
    explicit RpcClient(const std::string &server) : server_(server) {
        channel_ = grpc::CreateChannel(server_, grpc::InsecureChannelCredentials());
        stub_ = std::move(ServiceType::NewStub(channel_));
    }

    template<typename F, typename... Args>
    grpc::Status Call(F&& func, Args&&... args) {
        grpc::ClientContext ctx;
        return (stub_.get()->*func)(&ctx, std::forward<Args>(args)...);
    }

 private:
    std::string server_;
    std::shared_ptr<grpc::Channel> channel_;
    std::unique_ptr<StubType> stub_;
};

class KnnBenchmark final {
 public:
    using ClientType = RpcClient<KnnService>;
    using RateLimiter = cornerstone::common::RateLimiter;
    using RateLimiterFactory = cornerstone::common::RateLimiterFactory;

 public:
    KnnBenchmark();

    bool Init();
    void Run();
    void Stop();

 private:
    int32_t _LoadReq();
    void _WorkerRoutine(int32_t i);

 private:
    int32_t n_workers_ = 0, qps_limit_ = 0, interval_ = 1, times_ = 0;
    std::string model_name_;
    int64_t model_version_ = 0;
    std::string server_;
    std::string req_data_dir_;
    std::atomic_bool running_;
    std::atomic_int64_t qps_, cnt_success_, cnt_fail_, total_time_cost_;
    std::vector<std::unique_ptr<ClientType>> clients_;
    std::vector<std::thread> workers_;
    std::vector<std::unique_ptr<KnnRequest>> req_data_;
    std::shared_ptr<RateLimiter> rate_limiter_;
};

KnnBenchmark::KnnBenchmark() {
    server_ = FLAGS_server;
    n_workers_ = FLAGS_thread_num;
    qps_limit_ = FLAGS_qps;
    req_data_dir_ = FLAGS_d;
    interval_ = FLAGS_r > 0 ? FLAGS_r : 1;
    times_ = FLAGS_t > 0 ? FLAGS_t : 0;
    model_name_ = FLAGS_model_name;
    model_version_ = FLAGS_model_ver;

    std::atomic_init(&running_, false);
    std::atomic_init(&qps_, (int64_t)0);
    std::atomic_init(&cnt_success_, (int64_t)0);
    std::atomic_init(&cnt_fail_, (int64_t)0);
    std::atomic_init(&total_time_cost_, (int64_t)0);

    for (int32_t i = 0; i < n_workers_; ++i) {
        clients_.emplace_back(new ClientType(server_));
    }

    if (qps_limit_ > 0 ) {
        rate_limiter_ = RateLimiterFactory::get_token_bucket(0, qps_limit_);
        if (rate_limiter_) { rate_limiter_->set_rate(qps_limit_); }
    }

    std::srand(std::time(0));
}

void KnnBenchmark::Run() {
    // load request data
    LOG(INFO) << absl::StrFormat("Loading request data from %s ...", req_data_dir_);
    int32_t n_req_data = _LoadReq();
    LOG(INFO) << absl::StrFormat("Totally loaded %d request data.", n_req_data);
    if (n_req_data <= 0) {
        LOG(ERROR) << "Request dataset is empty, abort!";
        exit(-1);
    }

    // set model name
    if (!model_name_.empty()) {
        LOG(INFO) << absl::StrFormat("Setting model name to %s ...", model_name_);
        for (auto& req : req_data_) {
            req->mutable_model_spec()->set_name(model_name_);
        }
    }

    // set model version
    if (model_version_ > 0) {
        LOG(INFO) << absl::StrFormat("Setting model version to %ld ...", model_version_);
        for (auto& req : req_data_) {
            req->mutable_model_spec()->mutable_version()->set_value(model_version_);
        }
    }

    // start
    running_ = true;
    for (int32_t i = 0; i < n_workers_; ++i) {
        workers_.emplace_back(&KnnBenchmark::_WorkerRoutine, this, i);
    }

    // statistics
    int32_t cnt = 0;
    while (running_) {
        ::sleep(interval_);

        int64_t qps = qps_, n_success = cnt_success_, n_fail = cnt_fail_, total_time = total_time_cost_;
        qps_ = cnt_success_ = cnt_fail_ = total_time_cost_ = 0;
        double avg_time = (qps == 0 ? 0.0 : (double)total_time / (double)qps);
        double avail_ratio = (qps == 0 ? 0.0 : (double)n_success / (double)qps);
        std::cerr << absl::StrFormat("QPS: %ld\tAvail: %0.2lf%%\tAVG: %0.2lf",
                    qps, avail_ratio * 100, avg_time) << std::endl;

        if (times_ > 0 && ++cnt >= times_) { break; }
    }

    Stop();
}

void KnnBenchmark::_WorkerRoutine(int32_t i) {
    while (running_) {
        if (rate_limiter_ && !rate_limiter_->try_acquire(1)) {
            std::this_thread::yield();
            continue;
        }

        int32_t idx = rand() % req_data_.size();
        auto* cli = clients_[i].get();
        KnnResponse res;
        auto& req = *(req_data_[idx]);

        // run rpc
        auto start = std::chrono::high_resolution_clock::now();
        auto status = cli->Call(&KnnService::Stub::KnnPredSearch, req, &res);
        auto end = std::chrono::high_resolution_clock::now();
        auto cnt_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        total_time_cost_ += cnt_ms;

        ++qps_;
        if (status.ok()) {
            ++cnt_success_;
        } else {
            ++cnt_fail_;
        }
    }  // while
}

int32_t KnnBenchmark::_LoadReq() {
    for (fs::directory_iterator itr(req_data_dir_); itr != fs::directory_iterator(); ++itr) {
        if (!fs::is_regular_file(*itr)) { continue; }
        std::ifstream ifs(itr->path().string(), std::ios::in | std::ios::binary);
        if (!ifs) {
            LOG(ERROR) << absl::StrFormat("Fail to read file %s, skipping!", itr->path().string());
            continue;
        }
        std::unique_ptr<KnnRequest> req(new KnnRequest);
        if (!req->ParseFromIstream(&ifs)) {
            LOG(ERROR) << absl::StrFormat("Fail to parse file %s, skipping!", itr->path().string());
            continue;
        }
        req_data_.emplace_back(std::move(req));
    }  // for
    return req_data_.size();
}

void KnnBenchmark::Stop() {
    if (running_) {
        running_ = false;
        for (int32_t i = 0; i < n_workers_; ++i) {
            if (workers_[i].joinable()) { workers_[i].join(); }
        }
    }
}

namespace {
void check_args() {
    if (FLAGS_server.empty()) {
        LOG(ERROR) << "-server not set!";
        exit(-1);
    }
    if (FLAGS_thread_num <= 0) {
        LOG(ERROR) << "-thread_num must be greater than 0!";
        exit(-1);
    }
    if (!fs::exists(FLAGS_d) || !fs::is_directory(FLAGS_d)) {
        LOG(ERROR) << absl::StrFormat("Specified dir %s is not a valid directory!", FLAGS_d);
        exit(-1);
    }
}
}  // namespace

int main(int argc, char **argv) {
    GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = true;
    google::InitGoogleLogging(argv[0]);

    check_args();

    try {
        KnnBenchmark test;
        test.Run();

    } catch (const std::exception &ex) {
        LOG(ERROR) << "main exception: " << ex.what();
        exit(-1);
    } catch (...) {
        LOG(ERROR) << "main unknown exception!";
        exit(-1);
    }

    return 0;
}

