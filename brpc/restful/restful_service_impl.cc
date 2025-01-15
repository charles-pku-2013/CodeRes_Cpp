#include "restful_service_impl.h"
#include <butil/logging.h>

namespace ai_matrix {

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
        int ec = (it->second)(unresolved_path, cntl->request_attachment().to_string(), &out);
        if (ec) {
            os << "Requested restful api `" << query << "` fail, errcode=" << ec;
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

    // DEBUG
    // std::cout << cntl->http_request().unresolved_path() << std::endl;
    // std::cout << cntl->request_attachment() << std::endl;
    // cntl->http_request().uri().Print(std::cout);
    // std::cout << std::endl;
    // auto& uri = cntl->http_request().uri();
    // for (auto it = uri.QueryBegin(); it != uri.QueryEnd(); ++it) {
        // std::cout << "query: " << it->first << "\t" << it->second << std::endl;
    // }
    // cntl->response_attachment().append("Test OK");
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
    return "RestfulServiceImpl"; // TODO
}

#if 0
bool RestfulServer::Start(int port) {
    if (server_.AddService(&impl_,
                          brpc::SERVER_DOESNT_OWN_SERVICE,
                          "/api/* => HandleRequest") != 0) {
        return false;
    }
    if (server_.Start(port, &options_) != 0) {
        return false;
    }
    server_.RunUntilAskedToQuit();
    return true;
}

void RestfulServer::Stop() {
    server_.Stop(-1);
    server_.Join();
}
#endif

}  // namespace ai_matrix
