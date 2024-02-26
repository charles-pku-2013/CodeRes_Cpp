#include <iostream>
#include <json/json.h>
#include <hocon/config.hpp>
// #include <glog/logging.h>
#include "absl/strings/str_format.h"
// #include <boost/lexical_cast.hpp>

int main() {
    // std::string str = "{\"uploadid\": \"UP000000\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}";
    std::string str = "{name:chares,id:123,score:100}";   // FAIL
    auto conf = hocon::config::parse_string(str);
    std::string strJson = conf->root()->render(hocon::config_render_options(false, false, false, true));

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(strJson, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素
    {
        std::cerr << "Invalid json!" << std::endl;
    }

    Json::StyledWriter writer;
    std::string outStr = writer.write(root);
    std::cout << outStr << std::endl;

    return 0;
}

