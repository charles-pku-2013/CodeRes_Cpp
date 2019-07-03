#pragma once

#include <brpc/channel.h>
#include <string>
#include <memory>
#include <utility>
#include <iostream>

template<typename Request, typename Response, typename StubType>
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

