#include <glog/logging.h>
#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "absl/strings/str_cat.h"
#include "servable_manager.h"
#include "demo_dict.h"
#include "unistd.h"

namespace demo {

namespace fs = boost::filesystem;

DemoDict::~DemoDict() {
    // LOG(INFO) << "DemoDict destruct"; // core
    std::cout << "DemoDict destruct" << std::endl;
}

bool DemoDict::Load(const std::string &fname) {
    std::ifstream ifs(fname, std::ios::in);
    if (!ifs) {
        LOG(ERROR) << "DemoDict cannot open " << fname << " for reading!";
        return false;
    }

    DictType _dict;
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
            LOG(ERROR) << fname << " line " << linecnt << " Invalid: " << line;
            continue;
        }
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        boost::trim(key);
        boost::trim(value);
        if (key.empty() || value.empty()) {
            LOG(ERROR) << fname << " line " << linecnt << " Invalid: " << line;
            continue;
        }
        _dict[key] = value;
    }

    dict_.swap(_dict);
    return true;
}

namespace {
// 创建字典对象的工厂函数，要注册到ServableManager里
std::pair<bool, std::string>
CreateDemoDict(const std::string& path, const std::shared_ptr<common::ServableInfo> &info,
        std::shared_ptr<DemoDict>* pDict) {
    /*
     * path 到 version 级目录，如 xxx.../demo_dict/1/ 直接从这里面找数据文件
     * info 由 demo_dict/meta 文件所生成，为所有版本所共有，
     *      可以通过 info->info(map<string,string>) 获取所有配置信息
     */

    LOG(INFO) << "CreateDemoDict path: " << path;
    fs::path fdict = fs::path(path) / "dict.txt";
    if (!fs::exists(fdict))
    { return std::make_pair(false, absl::StrCat(fdict.string(), " does not exist!")); }

    std::shared_ptr<DemoDict> _pDict(new DemoDict);
    if (!_pDict->Load(fdict.string())) {
        return std::make_pair(false, "load data error");
    }

    ::sleep(5);  // 模拟长时间加载
    LOG(INFO) << "CreateDemoDict success!";

    *pDict = std::move(_pDict);
    return std::make_pair(true, "");
}

// optional 字典的数据检查函数，如数据完整性检查，看所需的数据文件是否都存在
// 扫描目录时若指定有检查函数，则会执行，若返回fale，则会跳过该版本目录
std::pair<bool, std::string>
CheckDemoDict(const std::string& path) {
    /*
     * path 到 version 级目录
     */
    if (!fs::exists(fs::path(path) / "done")) {
        return std::make_pair(false, "Incomplete dict data");
    }
    return std::make_pair(true, "");
}
}  // namespace

/*
 * 格式: REGISTER_SERVABLE_CREATOR(class_name, type, creator_function)
 * 其中class_name要与meta文件中的class_name一致
 */
REGISTER_SERVABLE_CREATOR("DemoDict", DemoDict, CreateDemoDict)
// REGISTER_SERVABLE_CHECKER("DemoDict", CheckDemoDict)

}  // namespace demo
