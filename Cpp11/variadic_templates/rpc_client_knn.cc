/*
# build:
bazel build -c opt //tensorflow_serving/example:knn_cli
# usage
knn_cli -server ip:port < reqfile
 */
#include <glog/logging.h>
#include <gflags/gflags.h>
#include <string>
#include <memory>
#include <iostream>
// #include <fstream>
// #include <thread>
// #include <atomic>

// bazel-genfiles/tensorflow_serving/apis
#include "tensorflow_serving/apis/knnpred_service.grpc.pb.h"
#include "tensorflow_serving/apis/knnpred_service.pb.h"

#include <grpcpp/grpcpp.h>

DEFINE_string(server, "", "server addr ip:port");

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

int main(int argc, char **argv) {
    GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = true;
    google::InitGoogleLogging(argv[0]);

    if (FLAGS_server.empty()) {
        LOG(ERROR) << "-server not set!";
        exit(-1);
    }

    try {
        KnnRequest req;
        KnnResponse res;

        if (!req.ParseFromIstream(&std::cin)) {
            LOG(ERROR) << "Fail to read request data!";
            exit(-1);
        }

        RpcClient<KnnService> cli(FLAGS_server);
        auto status = cli.Call(&KnnService::Stub::KnnPredSearch, req, &res);
        if (!status.ok()) {
            LOG(ERROR) << "Request failed! " << status.error_code() << ": " << status.error_message();
        }
        std::cout << res.DebugString() << std::endl;

    } catch (const std::exception &ex) {
        LOG(ERROR) << "main exception: " << ex.what();
        exit(-1);
    } catch (...) {
        LOG(ERROR) << "main unknown exception!";
        exit(-1);
    }

    return 0;
}
