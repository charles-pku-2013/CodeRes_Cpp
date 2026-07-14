#include <iostream>
#include <variant>
#include <string>

// 这是一个常用的辅助工具（C++17 语法），用于将多个 lambda 组合成一个重载对象
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

void demo2() {
    std::variant<int, double> v1 = 3.14;
    std::variant<std::string, bool> v2 = "激活状态";

    // 使用两个 auto& 接收参数，std::visit 会自动匹配
    std::visit([](const auto& arg1, const auto& arg2) {
        // 1. 获取剥离了 const 和 引用(&) 之后的纯净类型
        using T1 = std::decay_t<decltype(arg1)>;
        using T2 = std::decay_t<decltype(arg2)>;

        // 2. 使用 if constexpr 进行编译期类型分流
        if constexpr (std::is_same_v<T1, int> && std::is_same_v<T2, std::string>) {
            // 当 v1 是 int，v2 是 string 时走这里
            std::cout << "【分支 A】int 遇见了 string: " << arg1 << " -> " << arg2 << "\n";

        } else if constexpr (std::is_same_v<T1, double> && std::is_same_v<T2, bool>) {
            // 当 v1 是 double，v2 是 bool 时走这里
            std::cout << "【分支 B】double 遇见了 bool，结果是: " << (arg2 ? arg1 : 0.0) << "\n";

        } else {
            // 兜底的其他所有类型组合
            std::cout << "【其他分支】未特殊处理的类型组合\n";
        }
    }, v1, v2);

    // 输出: 第一个参数值: 3.14, 第二个参数值: 激活状态
}

int main() {
    std::variant<int, double, std::string> myVariant;

    // 定义一个访问者，里面包含针对每种类型的处理函数
    auto myVisitor = overloaded {
        [](int arg) { std::cout << "处理整数，乘以2: " << arg * 2 << "\n"; },
        [](double arg) { std::cout << "处理浮点数，保留一半: " << arg / 2.0 << "\n"; },
        [](const std::string& arg) { std::cout << "处理字符串，长度为: " << arg.length() << "\n"; }
    };

    // 测试 int
    myVariant = 10;
    std::visit(myVisitor, myVariant); // 输出: 处理整数，乘以2: 20

    // 测试 string
    myVariant = "OpenAI";
    std::visit(myVisitor, myVariant); // 输出: 处理字符串，长度为: 6

    return 0;
}
