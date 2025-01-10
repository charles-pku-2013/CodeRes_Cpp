/*
c++ -o /tmp/server *.cc -DBRPC_WITH_GLOG=0 -DGFLAGS_NS=google -D__const__=__unused__ -std=c++11 -DNDEBUG -O2 -pipe -fPIC -fno-omit-frame-pointer -lprotobuf -lbrpc -lpthread -ldl -lrt
 */

#include "restful_service_impl.h"
#include <unistd.h>
#include <thread>

using namespace ai_matrix;

int main() {
    brpc::ServerOptions options;
    options.idle_timeout_sec = -1;

    RestfulServer svr(options);
    svr.Service().RegisterHandler("test", [](const std::string& uri, const std::string& body, std::string* out)->int {
        out->append(uri).append("\t").append(body);
        return 0;
    });

    svr.Start(8000);
    // std::thread thr([&]{
        // svr.Start(8000);
    // });
    // ::sleep(1);

    // svr.Stop();

    return 0;
}

