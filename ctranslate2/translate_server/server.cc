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
/tmp/server -smodel 418M/spm.model -tmodel opus-mt-en-de-convert -split_svr http://127.0.0.1:7003/split -port 8000
/tmp/build/server -smodel 418M/spm.model -tmodel opus-mt-en-de-convert -worker_que_timeout 500 -split_svr http://127.0.0.1:7003/split
/tmp/build/server -smodel 418M/spm.model -tmodel 418M -worker_que_timeout 500 -split_svr http://127.0.0.1:7003/split

## send restful request
curl http://127.0.0.1:8000/api/translate -d '{"articles" : ["Hello world!", "How are you?"], "src" : "en", "dst" : "de"}'; echo
curl http://127.0.0.1:8000/api/translate -d '{"articles" : ["This is a machine translation program running on NPU of Huawei server. Hello. How are you today.", "All formatting is locale-independent by default. Use the format specifier to insert the appropriate number separator characters from the locale."], "src" : "en", "dst" : "de"}'; echo

curl http://127.0.0.1:8000/api/translate -d '{"articles" : ["您吃饭了吗？", "你好吗？"], "src" : "zh", "dst" : "en"}'; echo

response:
{"results":["Hallo Welt!","Wie geht es dir?"]}
 */

#include <curl/curl.h>
#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <glog/logging.h>
#include <json2pb/json_to_pb.h>
#include <json2pb/pb_to_json.h>

#include "restful_service_impl.h"
#include "task_queue.h"
#include "translator.h"
#include "model_wrapper.h"

DEFINE_string(model, "", "打包后的模型");
DEFINE_string(smodel, "", "sentencepiece分词用的模型");
DEFINE_string(tmodel, "", "ctranslate2翻译用的模型");
DEFINE_string(split_svr, "", "断句服务器地址");
DEFINE_string(device, "cpu", "计算设备 cpu(默认) 或 gpu(cuda)");
DEFINE_int32(n_devices, 0, "GPU处理器数量 (默认0自动设置)");
DEFINE_uint64(inter_threads, 1, "Same as 'inter_threads' in ctranslate2");
DEFINE_int32(port, 8000, "本服务器端口");
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

// const bool smodel_checker = gflags::RegisterFlagValidator(&FLAGS_smodel, check_empty_string_arg);
// const bool tmodel_checker = gflags::RegisterFlagValidator(&FLAGS_tmodel, check_empty_string_arg);
const bool split_svr_checker =
    gflags::RegisterFlagValidator(&FLAGS_split_svr, check_empty_string_arg);

const bool port_checker = gflags::RegisterFlagValidator(
    &FLAGS_port,
    [](const char* flagname, const int32_t value) -> bool { return value > 0 && value < 65536; });

}  // namespace

namespace {

class TranslateTaskItem : public TimeoutTaskItem {
 public:
    TranslateTaskItem(StringArray articles, std::string src_language, std::string dst_language,
                      Translator* translator)
        : articles_(std::move(articles)),
          src_language_(std::move(src_language)),
          dst_language_(std::move(dst_language)),
          translator_(translator) {}

    void jobRoutine() override {
        if (canceled_) {
            return;
        }
        try {
            for (auto& article : articles_) {
                auto result = translator_->Translate(article, src_language_, dst_language_);
                results_.emplace_back(std::move(result));
            }
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

    bool isCanceled() const {
        return canceled_;
    }

    static void cancelTask(TranslateTaskItem* task) {
        task->canceled_ = true;
    }

 private:
    StringArray      articles_;
    StringArray      results_;
    std::string      src_language_, dst_language_;
    std::string      err_msg_;
    Translator*      translator_ = nullptr;
    std::atomic_bool canceled_{false};
};

}  // namespace

// TEST
namespace {

butil::Status translate_without_queue(const std::string& uri, const std::string& body,
                                      std::string* out, brpc::Controller* cntl,
                                      TimeoutTaskQueue* _task_queue, Translator* translator) {
    auto& task_queue = *_task_queue;

    if (body.empty()) {
        return butil::Status(brpc::HTTP_STATUS_BAD_REQUEST, "Request body cannot be empty!");
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

    if (req.articles().empty()) {
        return butil::Status(brpc::HTTP_STATUS_BAD_REQUEST, "Request 'articles' cannot be empty");
    }

    std::vector<std::string> article_set(std::make_move_iterator(req.articles().begin()),
                                         std::make_move_iterator(req.articles().end()));

    try {
        for (auto& article : article_set) {
            auto result = translator->Translate(article, req.src(), req.dst());
            res.add_results(std::move(result));
        }
    } catch (const std::exception& ex) {
        return butil::Status(
            brpc::HTTP_STATUS_INTERNAL_SERVER_ERROR,
            fmt::format("Failed to translate '{}' for error: '{}'", body, ex.what()));
    }

    if (!json2pb::ProtoMessageToJson(res, out, json2pb::Pb2JsonOptions(), &err)) {
        return butil::Status(
            brpc::HTTP_STATUS_INTERNAL_SERVER_ERROR,
            fmt::format("Failed to convert TranslateResponse to json format: '{}'", err));
    }

    return butil::Status::OK();
}

}  // namespace

int main(int argc, char** argv) {
    google::ParseCommandLineFlags(&argc, &argv, true);

    google::InitGoogleLogging(argv[0]);
    google::LogToStderr();

    std::unique_ptr<ModelWrapper> model_wrapper;
    if (!FLAGS_model.empty()) {
        model_wrapper.reset(new ModelWrapper);
        auto models = model_wrapper->decrypt_model(FLAGS_model);
        FLAGS_tmodel = std::get<0>(models);
        FLAGS_smodel = std::get<1>(models);
    }

    if (FLAGS_n_workers == 0) {
        FLAGS_n_workers = std::thread::hardware_concurrency();
    }

    TimeoutTaskQueue task_queue(/* capacity */ FLAGS_worker_que_cap,
                                /* timeout_in_ms */ FLAGS_worker_que_timeout,
                                /* n_workers */ FLAGS_n_workers);
    task_queue.start();
    LOG(INFO) << "Initialized TaskQueue: " << task_queue.DebugString();

    // prepare curl
    curl_global_init(CURL_GLOBAL_ALL);

    // auto cleanup on finish
    auto cleanup_fn = [&](void*) {
        task_queue.stop();
        curl_global_cleanup();
    };
    std::unique_ptr<void, decltype(cleanup_fn)> cleanup((void*)1, cleanup_fn);

    LOG(INFO) << "Initializing translator...";
    std::unique_ptr<Translator> translator;
    try {
        translator.reset(new Translator(FLAGS_smodel, FLAGS_tmodel, FLAGS_split_svr));
    } catch (const std::exception& ex) {
        LOG(ERROR) << "Failed to init translator: " << ex.what();
        return -1;
    }
    LOG(INFO) << "Successfully initialized translator: " << translator->DebugString();

    /**
     * @brief 处理翻译请求逻辑实现
     *
     * @param uri 请问url中未被解析的部分
     * @param body  request body text
     * @param out   response text
     * @return   http code with error message
     */
    RestfulServiceImpl::Instance().RegisterHandler(
        "translate",
        [&translator, &task_queue](const std::string& uri, const std::string& body,
                                   std::string* out, brpc::Controller* cntl) -> butil::Status {
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

            if (req.articles().empty()) {
                return butil::Status(brpc::HTTP_STATUS_BAD_REQUEST,
                                     "Request 'articles' cannot be empty");
            }

            std::vector<std::string> article_set(std::make_move_iterator(req.articles().begin()),
                                                 std::make_move_iterator(req.articles().end()));

            auto task = std::make_shared<TranslateTaskItem>(std::move(article_set), req.src(),
                                                            req.dst(), translator.get());

            cntl->NotifyOnCancel(brpc::NewCallback(&TranslateTaskItem::cancelTask, task.get()));

            if (!task_queue.push(task)) {
                return butil::Status(
                    brpc::HTTP_STATUS_INTERNAL_SERVER_ERROR,
                    fmt::format("Failed to translate '{}' for task queue is full", body));
            }

            task->wait();  // wait for task done or timeout

            if (task->isCanceled()) {
                return butil::Status::OK();
            }

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

    // for testing performance without TimeoutTaskQueue
    RestfulServiceImpl::Instance().RegisterHandler(
        "test",
        std::bind(translate_without_queue, std::placeholders::_1, std::placeholders::_2,
                  std::placeholders::_3, std::placeholders::_4, &task_queue, translator.get()));

    // Add other restful api here

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
