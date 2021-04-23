/*
bazel build -c opt //tensorflow_serving/example:dp_rank_cli
 */
#include <gflags/gflags.h>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#include <grpcpp/grpcpp.h>
#include "tensorflow_serving/apis/prediction_service.grpc.pb.h"
#include "tensorflow_serving/apis/prediction_service.pb.h"
#include "tensorflow/core/example/example.pb.h"

DEFINE_string(server, "", "server addr ip:port");
DEFINE_string(in_file, "", "input req file");
DEFINE_bool(silent, false, "Do not print response");
DEFINE_bool(parsereq, false, "Parse request");
// DEFINE_string(out_file, "", "output res file");

using Request = tensorflow::serving::PredictRequest;
using Response = tensorflow::serving::PredictResponse;
using PredictionService = tensorflow::serving::PredictionService;

namespace {
// bool _check_server = gflags::RegisterFlagValidator(&FLAGS_server,
            // [](const char* flagname, const std::string& value){
    // if (value.empty()) {
        // std::cerr << flagname << " not set" << std::endl;
        // return false;
    // }
    // return true;
// });

std::shared_ptr<std::istream>
open_input(const std::string &fname, std::ios_base::openmode mode = std::ios_base::in) {
    std::shared_ptr<std::istream> ret;
    if (fname.empty() || fname == "-")
        ret.reset(&std::cin, [](std::istream*){});
    else
        ret.reset(new std::ifstream(fname, mode));
    return ret;
}

std::shared_ptr<std::ostream>
open_output(const std::string &fname, std::ios_base::openmode mode = std::ios_base::out) {
    std::shared_ptr<std::ostream> ret;
    if (fname.empty() || fname == "-")
        ret.reset(&std::cout, [](std::ostream*){});
    else
        ret.reset(new std::ofstream(fname, mode));
    return ret;
}

bool load_req(const std::string &fname, Request *req) {
    auto stream = open_input(fname, std::ios::in | std::ios::binary);
    if (!(*stream)) {
        std::cerr << "Cannot open file " << fname << " for reading!" << std::endl;
        return false;
    }
    return req->ParseFromIstream(stream.get());
}
} // namespace

class DpRankClient final {
 public:
    explicit DpRankClient(std::shared_ptr<grpc::Channel> channel)
            : stub_(PredictionService::NewStub(channel)),
              req_cnt_(0), success_cnt_(0) {}

    bool do_request(const Request &req, Response *res) {
        ++req_cnt_;

        grpc::ClientContext ctx;
        grpc::Status status = stub_->Predict(&ctx, req, res);
        if (!status.ok()) {
            std::cerr << status.error_code() << ": " << status.error_message()
                    << std::endl;
            return false;
        }

        ++success_cnt_;
        return true;
    }

    uint32_t req_count() const { return req_cnt_; }
    uint32_t success_count() const { return success_cnt_; }

 private:
    std::unique_ptr<PredictionService::Stub> stub_;
    std::atomic<uint32_t> req_cnt_;
    std::atomic<uint32_t> success_cnt_;
};

class ReqParser final {
    using Example = tensorflow::Example;

 public:
    explicit ReqParser(const Request& req)
            : req_(req) {}

    void parse() {
        auto& inputs = req_.inputs();
        auto it = inputs.begin();
        if (it == inputs.end()) {
            std::cerr << "Invalid req no content found!" << std::endl;
            return;
        }
        std::cerr << "proto key: " << it->first << std::endl;
        auto& proto = it->second;
        std::cout << "n_docs = " << proto.string_val_size() << std::endl;
    }

    void list_feature() {
        auto& inputs = req_.inputs();
        auto it = inputs.begin();
        if (it == inputs.end()) {
            std::cerr << "Invalid req no content found!" << std::endl;
            return;
        }
        std::cerr << "proto key: " << it->first << std::endl;
        auto& proto = it->second;
        for (int i = 0; i < proto.string_val_size(); ++i) {
            Example example;
            if (!example.ParseFromString(proto.string_val(i))) {
                std::cerr << "Failed to parse example " << i << std::endl;
                continue;
            }
            auto& feature_map = example.features().feature();
            std::cout << "features in example " << i << ":" << std::endl;
            for (auto& kv : feature_map) {
                std::cout << kv.first << std::endl;
            }
        } // for i
    }

 private:
    const Request& req_;
};

int main(int argc, char* argv[]) {
    using namespace std;

    GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);

    Request req;
    Response res;
    if (!load_req(FLAGS_in_file, &req)) {
        cerr << "Fail to load request!" << endl;
        return -1;
    }

    if (FLAGS_parsereq) {
        ReqParser parser(req);
        parser.parse();
        // parser.list_feature();
        return 0;
    }

    if (FLAGS_server.empty()) {
        std::cerr << "--server not set" << std::endl;
        return -1;
    }

    DpRankClient client(grpc::CreateChannel(
                 FLAGS_server, grpc::InsecureChannelCredentials()));
    if (!client.do_request(req, &res)) {
        cerr << "Request server fail!" << endl;
        return -1;
    }
    cout << res.DebugString() << endl;
    cerr << endl;
    cerr << "Performed requests: " << client.success_count() << " / " << client.req_count() << endl;

    return 0;
}
