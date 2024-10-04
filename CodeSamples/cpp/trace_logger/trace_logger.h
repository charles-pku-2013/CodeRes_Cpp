#pragma once
#include <memory>
#include <atomic>
#include <limits>
#include <type_traits>
#include <json/json.h>
#include <hocon/config.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes/constant.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/sources/channel_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
// #include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include "common/spinlock.h"
#include "absl/strings/str_cat.h"

namespace common {

// jsf也使用了boost::log, 为了不冲突定义channel
BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)

/*
 * Created for every request
 */
class TraceLogger {
    friend class TraceLoggerManager;
    friend class RequestTraceLoggerObj;
 public:
    using pointer = std::shared_ptr<TraceLogger>;

 public:
    explicit TraceLogger(const Json::Value& record)
            : record_(record) {}
    virtual ~TraceLogger();

    template<typename T>
    void AddRecord(const std::string& key, const T& val) {
        SpinLock::Holder holder(&lock_);
        record_[key] = val;
    }

    // 最多支持2层嵌套
    template<typename T>
    void AddRecord(const std::string& key1, const std::string& key2, const T& val) {
        SpinLock::Holder holder(&lock_);
        record_[key1][key2] = val;
    }

    template<typename Iter, typename = typename std::enable_if<!std::is_same<typename Iter::value_type, void>::value>::type>
    void AddRecord(Iter beg, Iter end) {
        SpinLock::Holder holder(&lock_);
        for (Iter it = beg; it != end; ++it)
        { record_[it->first] = it->second; }
    }

    template<typename Iter, typename = typename std::enable_if<!std::is_same<typename Iter::value_type, void>::value>::type>
    void AddRecord(Iter beg, Iter end, const std::string& prefix) {
        SpinLock::Holder holder(&lock_);
        for (Iter it = beg; it != end; ++it)
        { record_[absl::StrCat(prefix, ".", it->first)] = it->second; }
    }

    // 2层嵌套
    template<typename Iter, typename = typename std::enable_if<!std::is_same<typename Iter::value_type, void>::value>::type>
    void AddRecord(const std::string& key, Iter beg, Iter end) {
        SpinLock::Holder holder(&lock_);
        for (Iter it = beg; it != end; ++it)
        { record_[key][it->first] = it->second; }
    }

    // 直接编辑json内容, not thread safe
    Json::Value& ToJson()
    { return record_; }

    // 析构时自动调用，也可手动调用
    void Flush();

 protected:
    Json::Value record_;  // guarded by lock_
    SpinLock lock_;  // 支持并行写入
};

/*
 * wrapper for TraceLogger
 * 实际工程中使用这个，隐藏对logger是否为空判断
 */
class RequestTraceLoggerObj {
 public:
    RequestTraceLoggerObj() = default;
    RequestTraceLoggerObj(const TraceLogger::pointer& logger)
                    : logger_(logger) {}

    // 通用添加记录API
    template<typename... Args>
    void AddRecord(Args&&... args) {
        if (logger_)
        { logger_->AddRecord(std::forward<Args>(args)...); }
    }

    // 判断所持有的logger是否是空指针，实际使用时即使不判空也没问题
    operator bool() const
    { return logger_ ? true : false; }

    Json::Value* ToJson()
    { return logger_ ? &(logger_->record_) : nullptr; }

    void Flush()
    { if (logger_) { logger_->Flush(); } }

 private:
    TraceLogger::pointer logger_;
};

/*
 * logger管理类，全局单实例
 */
class TraceLoggerManager final {
    using file_sink_t = boost::log::sinks::asynchronous_sink<boost::log::sinks::text_file_backend>;
    using logger = boost::log::sources::channel_logger_mt<>;

 public:
    static TraceLoggerManager& Instance() {
        static TraceLoggerManager inst;
        return inst;
    }

    // 用hocon conf初始化
    bool Init(hocon::shared_config conf);

    // 析构时会自动调用，建议在程序结束前手动调用
    void Stop();

    void SetSampleRate(std::size_t value)
    { sample_rate_.store(value, std::memory_order_release); }

    size_t GetSampleRate() const {
        return sample_rate_.load(std::memory_order_acquire);
    }

    // for every req create a logger and kept in class context
    RequestTraceLoggerObj NewLogger();

    /*
     * 设置公共属性如ip, cluster, env ...
     * 之后通过NewLogger()获取的每一个logger对象都自带这些公共属性
     */
    template<typename T>
    void AddCommonRecord(const std::string& key, const T& val)
    { common_record_[key] = val; }

    std::string DebugString() const;

    logger& Logger() { return logger_; }

 private:
    void _Init(hocon::shared_config conf);

    bool inited_ = false;
    std::atomic_size_t req_cnt_{0};  // 请求计数
    // sample every N req, 0: disable, 1 (default): every req
    std::atomic<size_t> sample_rate_{1};

    std::string logdir_ = ".";
    std::string log_filename_ = "%Y%m%d_%H%M%S.log";  // log文件名格式，默认是时间戳
    std::string rotation_time_;  // in format "HH:MM:SS"  多长时间生成新log，比如每天0点就是: 00:00:00
    int64_t rotation_size_ = std::numeric_limits<int64_t>::max();
    int64_t max_log_files_ = std::numeric_limits<int64_t>::max();
    int64_t max_log_size_ = std::numeric_limits<int64_t>::max();
    int64_t min_free_space_ = 0;

    boost::shared_ptr<file_sink_t> file_sink_;
    logger logger_;

    Json::Value common_record_;  // ip, cluster, env ...

 private:
    static constexpr char LOG_CHANNEL[] = "req_trace";
    TraceLoggerManager() : logger_(boost::log::keywords::channel = LOG_CHANNEL) {}
    ~TraceLoggerManager();
    TraceLoggerManager(const TraceLoggerManager&) = delete;
    TraceLoggerManager(TraceLoggerManager&&) = delete;
    TraceLoggerManager& operator=(const TraceLoggerManager&) = delete;
    TraceLoggerManager& operator=(TraceLoggerManager&&) = delete;
};

}  // namespace common
