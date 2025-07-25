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
apt install -y libleveldb-dev libssl-dev libsnappy-dev
mkdir -p /tmp/build
cmake -B /tmp/build . -DWITH_GLOG=ON -DWITH_DEBUG_SYMBOLS=OFF -DCMAKE_INSTALL_PREFIX=/tmp/build/install
cd /tmp/build
make -j install
cd install
rsync -avi ./ /usr/local/
ldconfig

# build
protoc --cpp_out=. --proto_path=. translate.proto
c++ -o /tmp/server *.cc -DBRPC_WITH_GLOG=1 -DGFLAGS_NS=google -D__const__=__unused__ -std=c++17 -DNDEBUG -O2 -pipe -pthread -fPIC -fno-omit-frame-pointer -lprotobuf -lbrpc -ldl -lrt -lleveldb -lgflags -lglog -lssl -lcrypto -lfmt -lsentencepiece -lctranslate2

# 静态编译
注意.a库是顺序敏感的，libssl依赖于libcrypto，那么libssl.a必须放在libcrypto.a之前
现用动态链接编译出server
再执行 ldd server 得到其依赖的库
再把对应依赖库的静态链接库存到一个地方比如 /tmp/build/lib
protoc --cpp_out=. --proto_path=. translate.proto
c++ -o /tmp/server *.cc                                                              \
    /tmp/build/lib/libbrpc.a                                                         \
    /tmp/build/lib/libgflags.a                                                       \
    /tmp/build/lib/libglog.a                                                         \
    /tmp/build/lib/libleveldb.a                                                      \
    /tmp/build/lib/liblzma.a                                                         \
    /tmp/build/lib/libprotobuf.a                                                     \
    /tmp/build/lib/libsentencepiece.a                                                \
    /tmp/build/lib/libsnappy.a                                                       \
    /tmp/build/lib/libssl.a                                                          \
    /tmp/build/lib/libcrypto.a                                                       \
    /tmp/build/lib/libz.a                                                            \
    -DBRPC_WITH_GLOG=1 -DGFLAGS_NS=google -D__const__=__unused__ -std=c++17 -DNDEBUG \
    -O2 -pipe -pthread -fPIC -fno-omit-frame-pointer                                 \
    -ldl -lrt -lfmt -lctranslate2

# build with cmake
mkdir build && cd build
cmake ..
make -j

# test
## start server
/tmp/server -smodel 418M/spm.model -tmodel opus-mt-en-de-convert -port 8000
/tmp/build/server -smodel 418M/spm.model -tmodel opus-mt-en-de-convert -worker_que_timeout 500

## send restful request
curl http://127.0.0.1:8000/api/translate -d '{"text" : ["Hello world!", "How are you?"], "src" : "en", "dst" : "de"}'; echo

response:
{"results":["Hallo Welt!","Wie geht es dir?"]}
 */

#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <json2pb/json_to_pb.h>
#include <json2pb/pb_to_json.h>

#include "restful_service_impl.h"
#include "task_queue.h"
#include "translator.h"

DEFINE_string(smodel, "", "input file path");
DEFINE_string(tmodel, "", "output file path");
DEFINE_int32(port, 8000, "Server port");
DEFINE_uint32(n_workers, 0, "Num of worker threads. Default 0 for auto determing");
DEFINE_uint32(worker_que_cap, 100000, "Capacity of worker queue");
DEFINE_uint32(worker_que_timeout, 600 * 1000, "Timeout of worker queue in ms (default 10min)");

using namespace newtranx::ai_server;
using newtranx::TimeoutTaskItem;
using newtranx::TimeoutTaskQueue;

// arg checker
namespace {

bool check_empty_string_arg(const char* flagname, const std::string& value) {
    if (value.empty()) {
        std::cerr << fmt::format("Argument '{}' must be specified!", flagname) << std::endl;
        return false;
    }
    return true;
}

const bool smodel_checker = gflags::RegisterFlagValidator(&FLAGS_smodel, check_empty_string_arg);
const bool tmodel_checker = gflags::RegisterFlagValidator(&FLAGS_tmodel, check_empty_string_arg);

const bool port_checker = gflags::RegisterFlagValidator(
    &FLAGS_port,
    [](const char* flagname, const int32_t value) -> bool { return value > 0 && value < 65536; });

}  // namespace

namespace {

class TranslateTaskItem : public TimeoutTaskItem {
 public:
    TranslateTaskItem(StringArray orig_text, Translator* translator)
        : orig_text_(std::move(orig_text)), translator_(translator) {}

    void jobRoutine() override {
        try {
            results_ = translator_->Translate(orig_text_);
        } catch (const std::exception& ex) {
            err_msg_ = ex.what();
            throw;
        }
    }

    StringArray& results() {
        return results_;
    }

    std::string& err_msg() {
        return err_msg_;
    }

 private:
    StringArray orig_text_;
    StringArray results_;
    std::string err_msg_;
    Translator* translator_ = nullptr;
};

}  // namespace

int main(int argc, char** argv) {
    google::ParseCommandLineFlags(&argc, &argv, true);

    google::InitGoogleLogging(argv[0]);
    google::LogToStderr();

    if (FLAGS_n_workers == 0) {
        FLAGS_n_workers = std::thread::hardware_concurrency();
    }

    TimeoutTaskQueue task_queue(/* capacity */ FLAGS_worker_que_cap,
                                /* timeout_in_ms */ FLAGS_worker_que_timeout,
                                /* n_workers */ FLAGS_n_workers);
    task_queue.start();
    LOG(INFO) << "Initialized TaskQueue: " << task_queue.DebugString();

    // auto cleanup on finish
    auto                                        cleanup_fn = [&](void*) { task_queue.stop(); };
    std::unique_ptr<void, decltype(cleanup_fn)> cleanup((void*)1, cleanup_fn);

    LOG(INFO) << "Initializing translator...";
    std::unique_ptr<Translator> translator;
    try {
        translator.reset(new Translator(FLAGS_smodel, FLAGS_tmodel));
    } catch (const std::exception& ex) {
        LOG(ERROR) << "Failed to init translator: " << ex.what();
        return -1;
    }

    // 处理翻译请求逻辑实现
    RestfulServiceImpl::Instance().RegisterHandler(
        "translate",
        [&translator, &task_queue](const std::string& uri, const std::string& body,
                                   std::string* out) -> butil::Status {
            if (body.empty()) {
                return butil::Status(brpc::HTTP_STATUS_BAD_REQUEST,
                                     "Request body cannot be empty!");
            }

            TranslateRequest  req;
            TranslateResponse res;

            std::string err;
            if (!json2pb::JsonToProtoMessage(body, &req, json2pb::Json2PbOptions{}, &err)) {
                return butil::Status(brpc::HTTP_STATUS_BAD_REQUEST,
                                     fmt::format("Invalid request: '{}', err: '{}'", body, err));
            }

            if (!translator->IsSupportedLanguage(req.src())) {
                return butil::Status(
                    brpc::HTTP_STATUS_NOT_FOUND,
                    fmt::format("Requested src language '{}' is not supported", req.src()));
            }

            if (!translator->IsSupportedLanguage(req.dst())) {
                return butil::Status(
                    brpc::HTTP_STATUS_NOT_FOUND,
                    fmt::format("Requested dst language '{}' is not supported", req.dst()));
            }

            if (req.text().empty()) {
                return butil::Status(brpc::HTTP_STATUS_BAD_REQUEST,
                                     "Request 'text' cannot be empty");
            }

            std::vector<std::string> text_set(req.text().begin(), req.text().end());

            auto task = std::make_shared<TranslateTaskItem>(std::move(text_set), translator.get());
            if (!task_queue.push(task)) {
                return butil::Status(
                    brpc::HTTP_STATUS_INTERNAL_SERVER_ERROR,
                    fmt::format("Failed to translate '{}' for task queue is full", body));
            }

            task->wait();  // wait for task done or timeout

            if (task->status() == TimeoutTaskItem::Status::TIMEOUT) {
                return butil::Status(
                    brpc::HTTP_STATUS_REQUEST_TIMEOUT,
                    fmt::format("Failed to translate '{}' for task queue waitting timeout", body));
            }

            if (!task->err_msg().empty()) {
                return butil::Status(
                    brpc::HTTP_STATUS_INTERNAL_SERVER_ERROR,
                    fmt::format("Failed to translate '{}' for error: '{}'", body, task->err_msg()));
            }

            for (auto& str : task->results()) {
                res.add_results(std::move(str));
            }

            if (!json2pb::ProtoMessageToJson(res, out, json2pb::Pb2JsonOptions(), &err)) {
                return butil::Status(
                    brpc::HTTP_STATUS_INTERNAL_SERVER_ERROR,
                    fmt::format("Failed to convert TranslateResponse to json format: '{}'", err));
            }

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

    return 0;
}
