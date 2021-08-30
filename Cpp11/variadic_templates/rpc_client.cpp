#pragma once

#include <brpc/channel.h>
#include <string>
#include <memory>
#include <utility>
#include <iostream>

template<typename StubType>
class RpcClient {
 public:
    explicit RpcClient(const std::string &server, const std::string &load_balancer = "")
            : server_(server), load_balancer_(load_balancer) {}

    bool Init(const brpc::ChannelOptions &channel_options) {
        channel_options_ = channel_options;
        if (channel_.Init(server_.c_str(), load_balancer_.c_str(), &channel_options_) != 0) {
            std::cerr << "Fail to initialize rpc channel" << std::endl;  // DEBUG
            return false;
        }
        stub_.reset(new StubType(&channel_));
        return true;
    }

    // NOTE!!!
    template<typename F, typename... Args>
    bool Call(brpc::Controller *controller, F&& func, Args&&... args) {
        (stub_.get()->*func)(controller, std::forward<Args>(args)..., NULL);
        return controller->Failed() ? false : true;
    }

 private:
    std::string server_;
    std::string load_balancer_;
    brpc::Channel channel_;
    brpc::ChannelOptions channel_options_;
    std::unique_ptr<StubType> stub_;
};

// example
class KnnTest {
 public:
    using KnnStub = tensorflow::serving::KnnPredService_Stub;
    using Request = tensorflow::serving::KnnPredSearchRequest;
    using Response = tensorflow::serving::KnnPredSearchResult;
    using KnnPredService = tensorflow::serving::KnnPredService;
    using ClientType = XX::BRpcSingleClient<KnnPredService, Request, Response>;

    struct KnnClient : public ClientType {
        KnnClient() {
            ClientType::InitSubCallFunc(&KnnPredService::Stub::KnnPredSearch);
        }
    };

    void test1() {
        Request request;
        Response response;
        request.set_querystr("手机");
        request.set_top_k(10);
        request.mutable_model_spec()->set_name("knnpred_v3");

        rpc_client_.reset(new RpcClient<KnnStub>("11.7.159.141:8000"));
        brpc::ChannelOptions options;
        options.protocol = "h2:grpc";
        if (!rpc_client_->Init(options)) {
            cout << "knn client init fail!" << endl;
            return;
        }

        brpc::Controller cntl;
        rpc_client_->Call(&cntl, &KnnStub::KnnPredSearch, &request, &response);

        if (cntl.Failed()) {
            cout << cntl.ErrorText() << endl;
            return;
        }

        cout << response.DebugString() << endl;
    }

    void test() {
        Request request;
        Response response;
        request.set_querystr("手机");
        request.set_top_k(10);
        request.mutable_model_spec()->set_name("knnpred_v3");

        knn_client_.reset(new KnnClient);
        brpc::ChannelOptions options;
        options.protocol = "h2:grpc";
        if (!knn_client_->InitChannel("11.7.159.141:8000", "", options)) {
            cout << "knn client init fail!" << endl;
            return;
        }

        if (!knn_client_->GetData(request, &response)) {
            cout << "call knn server failed" << endl;
            return;
        }

        cout << response.DebugString() << endl;
    }

 private:
    std::unique_ptr<KnnClient> knn_client_;
    std::unique_ptr<RpcClient<KnnStub>> rpc_client_;
};
