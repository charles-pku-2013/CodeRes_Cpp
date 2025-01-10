#pragma once
#include <iostream>  // DEBUG
#include <brpc/server.h>
#include <brpc/restful.h>
#include <unordered_map>
// #include <json2pb/pb_to_json.h>
#include "restful.pb.h"

namespace ai_matrix {

class RestfulServiceImpl : public RestfulService {
public:
    using Handler = std::function<int(const std::string&,   /* url */
                                      const std::string&,   /* body */
                                      std::string*)>;       /* resp */
    using HandlerTable = std::unordered_map<std::string, Handler>;

public:
    void HandleRequest(google::protobuf::RpcController* cntl_base,
                       const HttpRequest*,
                       HttpResponse*,
                       google::protobuf::Closure* done);

    void RegisterHandler(const std::string& name, Handler handler);

    std::string DebugString() const;

private:
    HandlerTable handlers_;
};

class RestfulServer final {
public:
    explicit RestfulServer(brpc::ServerOptions options)
                          : options_(std::move(options))
    {}

    bool Start(int port);
    void Stop();

    // RestfulServiceImpl* Service()
    // { return impl_.get(); }
    RestfulServiceImpl& Service()
    { return impl_; }

private:
    brpc::Server server_;
    brpc::ServerOptions options_;
    RestfulServiceImpl impl_;
    // std::unique_ptr<RestfulServiceImpl> impl_;
};

}  // namespace ai_matrix

