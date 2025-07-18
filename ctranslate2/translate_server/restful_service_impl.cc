#include "restful_service_impl.h"
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
        return;
    }

    butil::IOBufBuilder os;
    auto pos = unresolved_path.find_first_of('/');
    std::string query = unresolved_path.substr(0, pos);
    auto it = handlers_.find(query);
    if (it == handlers_.end()) {
        os << "Requested restful api `" << query << "` does not exist!";
        os.move_to(cntl->response_attachment());
        return;
    }

    try {
        std::string out;
        auto ec = (it->second)(unresolved_path, cntl->request_attachment().to_string(), &out);
        if (ec) {
            os << "Requested restful api `" << query << "` fail, errcode=" << ec << " " << ec.message();
            os.move_to(cntl->response_attachment());
        } else {
            cntl->response_attachment().append(out);
        }
    } catch (const std::exception& ex) {
        os << "Requested restful api `" << query << "` fail: " << ex.what();
        os.move_to(cntl->response_attachment());
    } catch (...) {
        os << "Requested restful api `" << query << "` fail for unknown reason!";
        os.move_to(cntl->response_attachment());
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
