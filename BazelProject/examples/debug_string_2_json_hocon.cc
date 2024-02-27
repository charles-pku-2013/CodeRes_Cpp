#include <iostream>
#include <json/json.h>
#include <hocon/config.hpp>
// #include <glog/logging.h>
#include "absl/strings/str_format.h"
// #include <boost/lexical_cast.hpp>

Json::Value DebugString_2_Json(const std::string& debug_string) {
    hocon::shared_config hocon_str;
    try {
        hocon_str = hocon::config::parse_string(debug_string);
    } catch (...) {
        LOG(ERROR) << absl::StrFormat("Invalid debug string `%s`", debug_string);
        return Json::Value();
    }

    if (!hocon_str) {
        LOG(ERROR) << absl::StrFormat("Invalid debug string `%s`", debug_string);
        return Json::Value();
    }

    std::string json_str = hocon_str->root()->render(hocon::config_render_options(false, false, false, true));
    Json::Reader reader;
    Json::Value item;

    if (!reader.parse(json_str, item)) {
        LOG(ERROR) << absl::StrFormat("Invalid debug string `%s`", debug_string);
        return Json::Value();
    }

    return item;
}

int main() {
    std::string str = "{name:chares,id:123,score:100}";
    Json::Value root = DebugString_2_Json(str);

    Json::StyledWriter writer;
    std::string outStr = writer.write(root);
    std::cout << outStr << std::endl;

    return 0;
}

