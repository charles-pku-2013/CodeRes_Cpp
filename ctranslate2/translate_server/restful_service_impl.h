#pragma once
#include <brpc/server.h>
#include <brpc/restful.h>
#include <unordered_map>
#include <system_error>
#include "translate.pb.h"

namespace newtranx {
namespace ai_server {

class RestfulServiceImpl : public RestfulService {
public:
    using Handler = std::function<std::error_code(const std::string&,   /* url */
                                                  const std::string&,   /* body */
                                                  std::string*)>;       /* resp */
    using HandlerTable = std::unordered_map<std::string, Handler>;

public:
    // singleton
    static RestfulServiceImpl& Instance();

    void HandleRequest(google::protobuf::RpcController* cntl_base,
                       const HttpRequest*,
                       HttpResponse*,
                       google::protobuf::Closure* done);

    void RegisterHandler(const std::string& name, Handler handler);

    bool Build(brpc::Server* server);

    std::string DebugString() const;

private:
    RestfulServiceImpl() = default;
    RestfulServiceImpl(const RestfulServiceImpl&) = delete;
    RestfulServiceImpl& operator=(const RestfulServiceImpl&) = delete;

    HandlerTable handlers_;
};

}  // namespace ai_server
}  // namespace newtranx

