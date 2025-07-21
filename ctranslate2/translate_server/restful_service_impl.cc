#include "restful_service_impl.h"
#include <system_error>
#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <glog/logging.h>

namespace newtranx {
namespace ai_server {

RestfulServiceImpl& RestfulServiceImpl::Instance() {
    static RestfulServiceImpl inst;
    return inst;
}

void RestfulServiceImpl::HandleRequest(google::protobuf::RpcController* cntl_base,
                                       const HttpRequest*,
                                       HttpResponse*,
                                       google::protobuf::Closure* done) {
    brpc::ClosureGuard done_guard(done);
    brpc::Controller* cntl =
        static_cast<brpc::Controller*>(cntl_base);

    const std::string& unresolved_path = cntl->http_request().unresolved_path();
    if (unresolved_path.empty()) {
        cntl->response_attachment().append("Empty request!");
        cntl->http_response().set_status_code(brpc::HTTP_STATUS_BAD_REQUEST);
        return;
    }

    butil::IOBufBuilder os;
    auto pos = unresolved_path.find_first_of('/');
    std::string query = unresolved_path.substr(0, pos);
    auto it = handlers_.find(query);
    if (it == handlers_.end()) {
        os << fmt::format("Requested restful api '{}' does not exist!", query);
        os.move_to(cntl->response_attachment());
        cntl->http_response().set_status_code(brpc::HTTP_STATUS_SERVICE_UNAVAILABLE);
        return;
    }

    try {
        std::string out;
        auto status = (it->second)(unresolved_path, cntl->request_attachment().to_string(), &out);
        if (status.ok()) {
            cntl->response_attachment().append(out);
        } else {
            os << fmt::format("Requested restful api '{}' fail, error: '{}'",
                              query, status.error_str());
            os.move_to(cntl->response_attachment());
            cntl->http_response().set_status_code(status.error_code());
        }
    } catch (const std::exception& ex) {
        os << fmt::format("Requested restful api '{}' fail: '{}'", query, ex.what());
        os.move_to(cntl->response_attachment());
        cntl->http_response().set_status_code(brpc::HTTP_STATUS_INTERNAL_SERVER_ERROR);
    } catch (...) {
        os << fmt::format("Requested restful api '{}' fail unknown exception!", query);
        cntl->http_response().set_status_code(brpc::HTTP_STATUS_INTERNAL_SERVER_ERROR);
    }
}

void RestfulServiceImpl::RegisterHandler(const std::string& name, Handler handler) {
    handlers_[name] = std::move(handler);
}

bool RestfulServiceImpl::Build(brpc::Server* server) {
    if (server->AddService(this,
                          brpc::SERVER_DOESNT_OWN_SERVICE,
                          "/api/* => HandleRequest") != 0) {
        return false;
    }
    return true;
}

std::string RestfulServiceImpl::DebugString() const {
    return "RestfulServiceImpl";
}

}  // namespace ai_server
}  // namespace newtranx
