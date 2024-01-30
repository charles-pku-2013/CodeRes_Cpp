#include <iostream>
#include <cctype>
#include <ctime>
#include <sstream>
// #include <boost/core/null_deleter.hpp>
#include <boost/lexical_cast.hpp>
#include <glog/logging.h>
#include "absl/strings/str_format.h"
#include "request_trace_logger.h"

namespace KKK {

namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace logging = boost::log;

RequestTraceLoggerManager::~RequestTraceLoggerManager()
{ Stop(); }

bool RequestTraceLoggerManager::Init(hocon::shared_config conf) {
    try {
        _Init(conf);
    } catch (const std::exception& ex) {
        LOG(ERROR) << "RequestTraceLoggerManager init fail: " << ex.what();
        return false;
    } catch (...) {
        LOG(ERROR) << "RequestTraceLoggerManager init fail!";
        return false;
    }
    inited_ = true;
    return true;
}

void RequestTraceLoggerManager::_Init(hocon::shared_config conf) {
    if (!conf) {
        throw std::runtime_error("RequestTraceLoggerManager init fail for NULL conf");
    }

    sample_rate_ = conf->has_path("sample_rate") ? conf->get_long("sample_rate") : sample_rate_;

    auto get_size_conf = [](std::string str_size)->int64_t {
        int64_t size = std::numeric_limits<int64_t>::max();
        int64_t unit = 1;
        char ch = str_size.back();
        if (std::isalpha(ch)) {
            str_size.pop_back();
            switch (ch) {
                case 'k':
                case 'K': {
                    unit = 1024;
                    break;
                }
                case 'm':
                case 'M': {
                    unit = 1024 * 1024;
                    break;
                }
                case 'g':
                case 'G': {
                    unit = 1024 * 1024 * 1024;
                    break;
                }
                default:
                    throw std::runtime_error(absl::StrFormat("Unsupported log file size: %s", str_size));
            }  // switch
        }  // if ch
        size = boost::lexical_cast<int64_t>(str_size);
        size *= unit;
        if (size <= 0)
        { throw std::runtime_error(absl::StrFormat("Invalid log file size: %s", str_size)); }
        return size;
    };

    log_filename_ = conf->has_path("filename") ? conf->get_string("filename") : log_filename_;
    logdir_ = conf->has_path("logdir") ? conf->get_string("logdir") : logdir_;
    rotation_size_ = conf->has_path("rotation_size") ? get_size_conf(conf->get_string("rotation_size")) : rotation_size_;
    file_sink_ = boost::make_shared<file_sink_t>(
        keywords::enable_final_rotation = false,
        keywords::file_name = boost::filesystem::path(logdir_) / log_filename_,  // log文件名格式,必须包含路径,会自动创建路径
        keywords::rotation_size = rotation_size_  // 单个日志文件大小
    );

    if (conf->has_path("rotation_time"))
    { rotation_time_ = conf->get_string("rotation_time"); }
    if (!rotation_time_.empty()) {
        std::string format = "%H:%M:%S";
        std::istringstream ss(rotation_time_);
        std::tm tm{};
        ss >> std::get_time(&tm, format.c_str());
        if (ss.fail()) {
            throw std::runtime_error(absl::StrFormat("Invalid log rotation time: %s", rotation_time_));
        }
        file_sink_->locked_backend()->set_time_based_rotation(sinks::file::rotation_at_time_point(tm.tm_hour, tm.tm_min, tm.tm_sec));
    }

    // Set up where the rotated files will be stored, 和正在记录的log路径是相互独立设置的
    max_log_files_ = conf->has_path("max_log_files") ? conf->get_long("max_log_files") : max_log_files_;
    max_log_size_ = conf->has_path("max_log_size") ? get_size_conf(conf->get_string("max_log_size")) : max_log_size_;
    min_free_space_ = conf->has_path("min_free_space") ? get_size_conf(conf->get_string("min_free_space")) : min_free_space_;
    file_sink_->locked_backend()->set_file_collector(sinks::file::make_collector(
        keywords::target = logdir_,  // 存储log文件的路径，不存在会自动创建
        keywords::max_files = max_log_files_,  // 最多log文件数量
        keywords::max_size = max_log_size_,              // maximum total size of the stored files, in bytes
        keywords::min_free_space = min_free_space_       // minimum free space on the drive, in bytes (SEEMS NOT WORK)
    ));

    file_sink_->locked_backend()->auto_flush(true);

    // Upon restart, scan the target directory for files matching the file_name pattern
    file_sink_->locked_backend()->scan_for_files();

    file_sink_->set_filter(channel == LOG_CHANNEL);

    logging::core::get()->add_sink(file_sink_);
}

RequestTraceLoggerObj RequestTraceLoggerManager::NewLogger() {
    if ( !inited_ || sample_rate_ == 0) { return RequestTraceLogger::pointer(); }   // disable logging
    return (req_cnt_++ % sample_rate_ == 0 ? std::make_shared<RequestTraceLogger>(common_record_) : RequestTraceLogger::pointer());
}

void RequestTraceLoggerManager::Stop() {
    inited_ = false;
    if (file_sink_) {
        logging::core::get()->remove_all_sinks();
        // Required if using async
        // Break the feeding loop
        file_sink_->stop();
        // Flush all log records that may have left buffered
        file_sink_->flush();
        file_sink_.reset();
    }
}

std::string RequestTraceLoggerManager::DebugString() const {
    return absl::StrFormat("{sample_rate:%lu,logdir:%s,log_filename:%s,rotation_size:%ld,rotation_time:%s,"
                                "max_log_files:%ld,max_log_size:%ld,min_free_space:%ld}",
                                sample_rate_, logdir_, log_filename_, rotation_size_,
                                rotation_time_.empty() ? "not_set" : rotation_time_,
                                max_log_files_, max_log_size_, min_free_space_);
}

RequestTraceLogger::~RequestTraceLogger()
{ Flush(); }

void RequestTraceLogger::Flush() {
    SpinLock::Holder holder(&lock_);
    if (!record_.empty()) {
        auto& logger = RequestTraceLoggerManager::Instance().Logger();
        Json::FastWriter writer;
        writer.omitEndingLineFeed();
        BOOST_LOG(logger) << writer.write(record_);
        record_.clear();
    }
}

}  // namespace KKK

