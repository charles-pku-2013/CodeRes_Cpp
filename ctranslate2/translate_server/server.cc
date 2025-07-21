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
cmake -B /tmp/build . -DWITH_GLOG=ON -DWITH_DEBUG_SYMBOLS=OFF -DCMAKE_INSTALL_PREFIX=/tmp/build/install
cd /tmp/build
make -j install
cd install
rsync -avi ./ /usr/local/
ldconfig

# build this app
c++ -o /tmp/server *.cc -DBRPC_WITH_GLOG=1 -DGFLAGS_NS=google -D__const__=__unused__ -std=c++17 -DNDEBUG -O2 -pipe -pthread -fPIC -fno-omit-frame-pointer -lprotobuf -lbrpc -ldl -lrt -lleveldb -lgflags -lglog -lssl -lcrypto -lfmt -lsentencepiece -lctranslate2

# test
/tmp/server -s_model 418M/spm.model -t_model opus-mt-en-de-convert
curl http://127.0.0.1:8000/api/translate -d 'Hello world!'
 */

#include "restful_service_impl.h"
#include "translator.h"
#include <fmt/base.h>
#include <fmt/ranges.h>
#include <fmt/format.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

DEFINE_string(s_model, "", "input file path");
DEFINE_string(t_model, "", "output file path");
DEFINE_int32(port, 8000, "Server port");

using namespace newtranx::ai_server;

// arg checker
namespace {

bool check_empty_string_arg(const char* flagname, const std::string& value) {
    if (value.empty()) {
        std::cerr << fmt::format("Argument '{}' must be specified!", flagname) << std::endl;
        return false;
    }
    return true;
}

const bool s_model_checker = gflags::RegisterFlagValidator(&FLAGS_s_model, check_empty_string_arg);
const bool t_model_checker = gflags::RegisterFlagValidator(&FLAGS_t_model, check_empty_string_arg);

const bool port_checker = gflags::RegisterFlagValidator(&FLAGS_port,
                          [](const char* flagname, const int32_t value)->bool {
    return value > 0 && value < 65536;
});

}  // namespace

int main(int argc, char **argv) {
    google::ParseCommandLineFlags(&argc, &argv, true);

    google::InitGoogleLogging(argv[0]);
    google::LogToStderr();

    try {
        LOG(INFO) << "Initializing translator...";
        Translator translator(FLAGS_s_model, FLAGS_t_model);

        // 处理翻译请求逻辑实现
        RestfulServiceImpl::Instance().RegisterHandler("translate",
                [&translator](const std::string& uri, const std::string& body, std::string* out)->butil::Status {
            if (body.empty()) {
                return butil::Status(brpc::HTTP_STATUS_BAD_REQUEST, "Request body cannot be empty!");
            }

            *out = translator.Translate(body);
            return butil::Status::OK();
        });

        brpc::Server server;
        if (!RestfulServiceImpl::Instance().Build(&server)) {
            LOG(ERROR) << "Failed to build server!";
            return -1;
        }

        brpc::ServerOptions options;
        options.idle_timeout_sec = -1;

        if (server.Start(FLAGS_port, &options) != 0) {
            LOG(ERROR) << "Fail to start restful server!";
            return -1;
        }

        server.RunUntilAskedToQuit();

    } catch (const std::exception& ex) {
        LOG(ERROR) << "Server caught exception: " << ex.what();
        return -1;
    }

    return 0;
}

