/*
# Remove all protobuf related packages
# find them by `apt search proto`

# build and install protobuf-3.14.0 for sentence
./autogen
./configure --prefix=/usr
make -j
make install
ldconfig

# build and install brpc-1.8.0
apt install -y libleveldb-dev libssl-dev
mkdir -p /tmp/build
cmake -B /tmp/build . -DWITH_GLOG=ON -DCMAKE_INSTALL_PREFIX=/tmp/build/install
cd /tmp/build
make -j install
cd install
rsync -avi ./ /usr/local/
ldconfig

# build this app
c++ -o /tmp/server *.cc -DBRPC_WITH_GLOG=1 -DGFLAGS_NS=google -D__const__=__unused__ -std=c++17 -DNDEBUG -O2 -pipe -pthread -fPIC -fno-omit-frame-pointer -lprotobuf -lbrpc -ldl -lrt -lleveldb -lgflags -lglog -lssl -lcrypto -lfmt -lsentencepiece -lctranslate2

# test
curl http://127.0.0.1:8000
curl http://127.0.0.1:8000/api/test -d 'hello'
 */

#include "restful_service_impl.h"
#include "translator.h"
#include <gflags/gflags.h>

using namespace newtranx::ai_server;

int main(int argc, char **argv) {
    google::ParseCommandLineFlags(&argc, &argv, true);

    google::InitGoogleLogging(argv[0]);
    google::LogToStderr();

    // test translator
    // try {
        // Translator translator(argv[1], argv[2]);
    // } catch (const std::exception& ex) {
        // std::cerr << "Test error: " << ex.what() << std::endl;
    // }

    RestfulServiceImpl::Instance().RegisterHandler("test",
            [](const std::string& uri, const std::string& body, std::string* out)->std::error_code {
        out->append(uri).append("\t").append(body);
        return std::error_code();
    });

    brpc::Server server;
    if (!RestfulServiceImpl::Instance().Build(&server)) {
        std::cout << "Failed to build server!" << std::endl;
        return -1;
    }

    brpc::ServerOptions options;
    options.idle_timeout_sec = -1;

    if (server.Start(8000, &options) != 0) {
        std::cout << "Fail to start restful server!" << std::endl;
        return -1;
    }

    server.RunUntilAskedToQuit();

    return 0;
}

