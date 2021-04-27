#include <hocon/config.hpp>

// remember try ... catch ...
void demo(hocon::shared_config conf) {
    // 打印成一行
    std::cout << "conf: " << conf->root()->render(hocon::config_render_options::concise()) << std::endl;
    // 原样输出
    std::cout << "conf: " << conf->root()->render(hocon::config_render_options(false, false)) << std::endl;
    // 遍历
    auto keys = conf->root()->key_set();
    for (const auto &key : keys) {
        std::cout << key << std::endl;
        auto s_conf = conf->get_config(key);  // or conf->root()->at_key(key)
        std::cout << s_conf->root()->render(hocon::config_render_options(false, false)) << std::endl;
    }
}


