/*
 * 读取配置文件格式如 key = val
 */
#pragma once
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <unordered_map>
#include <fstream>
#include <glog/logging.h>

namespace demo {

class ConfigReader final {
 public:
    using CfgDict = std::unordered_map<std::string, std::string>;

    bool load_config(const std::string &fanme);

    std::string DebugString() const {
        std::ostringstream oss;
        for (const auto &kv : config_)
            oss << kv.first << "=" << kv.second << " ";
        return oss.str();
    }

    const CfgDict& all_configs() const { return config_; }

    bool has_config(const std::string &name) const
    { return config_.count(name) > 0; }

    template<typename T>
    T get_config(const std::string &name, const T& default_val = T()) const {
        auto it = config_.find(name);
        if (it == config_.end())
        { return default_val; }

        T retval = default_val;
        try {
            retval = boost::lexical_cast<T>(it->second);
        } catch (const std::exception &ex) {
            LOG(ERROR) << "Get config fail: "
                    << it->first << "=" << it->second << " " << ex.what();
            return default_val;
        }
        return retval;
    }

    template<typename T>
    bool get_config(const std::string &name, T *out, std::function<bool(const std::string&, T*)> &&parser) {
        auto it = config_.find(name);
        if (it == config_.end())
        { return false; }
        return parser(it->second, out);
    }

 private:
    CfgDict config_;
};

inline
bool ConfigReader::load_config(const std::string &fname) {
    std::ifstream ifs(fname, std::ios::in);
    if (!ifs) { return false; }

    CfgDict _config;
    std::string line;
    uint32_t linecnt = 0;
    while (std::getline(ifs, line)) {
        ++linecnt;
        std::string::size_type pos = line.find('#');
        if (pos != std::string::npos)
        { line.resize(pos); }

        boost::trim(line);
        if (line.empty()) { continue; }

        pos = line.find('=');
        if (pos == std::string::npos || pos == 0) {
            LOG(ERROR) << fname << " line " << linecnt << " Invalid config: " << line;
            continue;
        }
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        boost::trim(key);
        boost::trim(value);
        if (key.empty() || value.empty()) {
            LOG(ERROR) << fname << " line " << linecnt << " Invalid config: " << line;
            continue;
        }
        _config[key] = value;
    }

    config_.swap(_config);

    return true;
}

}  // namespace demo
