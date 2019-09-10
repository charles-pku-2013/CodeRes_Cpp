#pragma once
#include <errno.h>
#include <boost/thread.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include <string>
#include <atomic>
#include <memory>
#include <utility>
#include <vector>
#include "kernels/algo/adapters/predictor/predictor_factory.h"
#include "common/brpc_client/client.h"
#include "tensorflow_serving/apis/prediction_service.pb.h"

namespace jdsearch {
namespace searcher {

class TFSPredictor final : public Predictor {
 public:
    using PredictionStub = tensorflow::serving::PredictionService_Stub;
    using Request = tensorflow::serving::PredictRequest;
    using Response = tensorflow::serving::PredictResponse;
    using PredictionService = tensorflow::serving::PredictionService;
    using ClientType = jdsearch::BRpcSingleClient<PredictionService, Request, Response>;

    class TfsClient final : public boost::shared_lockable_adapter<boost::shared_mutex> {
     public:
        enum Status {
            FAIL = -1,
            SUCCESS = 0,
            BUSY = 1,
        };

     public:
        TfsClient(const std::string &server_addr, const std::string &load_balancer)
                : server_addr_(server_addr), load_balancer_(load_balancer) {}

        bool Init() {
            boost::unique_lock<TfsClient> lck(*this);
            return _Init();
        }

        bool Reset() {
            if (available_) { return true; }
            boost::unique_lock<TfsClient> lck(*this);
            if (available_) { return true; }  // double check
            return _Init();
        }

        int GetData(const Request &req, Response *res, int timeout,
                            std::string *err_msg) {
            boost::shared_lock<TfsClient> lck(*this, boost::defer_lock);
            if (!lck.try_lock()) { return BUSY; }
            bool status = client_->GetData(req, res, timeout, err_msg);
            available_ = status;
            return status ? SUCCESS : FAIL;
        }

        bool available() const { return available_; }

        std::string DebugString() const {
            std::ostringstream oss;
            oss << "TfsClient: server=" << server_addr_ << " load_balancer=" << load_balancer_
                << " available=" << available_;
            return oss.str();
        }

     private:
        bool _Init() {
            brpc::ChannelOptions options;
            options.protocol = "h2:grpc";
            client_.reset(new ClientType);
            client_->InitSubCallFunc(&PredictionService::Stub::Predict);
            bool status = client_->InitChannel(server_addr_, load_balancer_, options);
            available_ = status;
            return status;
        }

        std::unique_ptr<ClientType> client_;
        std::string server_addr_;
        std::string load_balancer_;
        std::atomic_bool available_;
    };

    class ClientPool final {
     public:
        bool Add(const std::string &server_addr, const std::string &load_balancer) {
            std::unique_ptr<TfsClient> client(new TfsClient(server_addr, load_balancer));
            if (!client->Init()) { return false; }
            clients_.emplace_back(std::move(client));
            return true;
        }

        int GetData(const Request &req, Response *res, int timeout,
                    std::string *err_msg) {
            for (std::size_t i = 0; i < Size(); ++i) {
                TfsClient* client = GetClient();
                if (!client->available()) { continue; }
                int status = client->GetData(req, res, timeout, err_msg);
                if (status == TfsClient::SUCCESS) { return TfsClient::SUCCESS; }
                if (status == TfsClient::BUSY) { continue; }
                if (status == TfsClient::FAIL) {
                    client->Reset();  // no check return status
                    return TfsClient::FAIL;
                }
            }
            return TfsClient::BUSY;
        }

        TfsClient* GetClient() {
            return clients_[cnt_++ % Size()].get();
        }

        std::size_t Size() const { return clients_.size(); }

     private:
        std::vector<std::unique_ptr<TfsClient>> clients_;
        std::size_t cnt_ = 0;
    };

 public:
    // Called by tfs
    bool Init(const std::string& base_path) override;

    bool Predict(const Request &req, Response *res, int timeout = 0,
            std::string *err_msg = nullptr);  // timout in ms

    std::string DebugString() const override
    { return "TFSPredictor " + Predictor::DebugString(); }

 private:
    ClientPool client_pool_;
};


}  // namespace searcher
}  // namespace jdsearch
