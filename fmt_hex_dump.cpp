/*
c++ -o /tmp/test fmt_hex_dump.cpp -I/opt/homebrew/Cellar/fmt/11.2.0/include -L/opt/homebrew/Cellar/fmt/11.2.0/lib -lfmt -std=c++17 -g
 */
#include <fmt/base.h>
#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/os.h>
#include <fmt/ostream.h>
#include <fmt/printf.h>
#include <fmt/ranges.h>
#include <fmt/std.h>

#include <iostream>

// void hex_dump(const void* data, std::size_t size) {
//     // 1. 强制转换为 uint8_t 指针，确保按单字节处理且无符号扩展
//     auto begin = static_cast<const std::uint8_t*>(data);
//     auto end = begin + size;

//     // 2. 使用 fmt::join 配合格式化说明符
//     // {:02x} 表示：16进制显示、宽度为2、不足补0
//     fmt::print("[{:02x}]\n", fmt::join(begin, end, " "));
// }

/**
 * @brief 模拟 gdb 格式查看内存
 * @param data 内存起始指针
 * @param size 要查看的字节长度
 */
void hex_dump(const void* data, size_t size) {
    const auto*  ptr = static_cast<const unsigned char*>(data);
    const size_t row_size = 16;  // 每行显示16个字节

    for (size_t i = 0; i < size; i += row_size) {
        // 1. 打印当前行起始地址 (类似 gdb 的地址显示)
        fmt::print(stderr, fg(fmt::color::gray),
                   "{:#018x}:  ", reinterpret_cast<uintptr_t>(ptr + i));

        // 2. 收集当前行的字节，准备打印十六进制
        size_t current_row_len = std::min(row_size, size - i);

        // 打印十六进制部分
        for (size_t j = 0; j < row_size; ++j) {
            if (j < current_row_len) {
                fmt::print(stderr, "{:02x} ", ptr[i + j]);
            } else {
                fmt::print(stderr, "   ");  // 长度不足时补齐空格
            }
            if (j == 7) {
                fmt::print(stderr, " ");  // 在第8个字节后加个额外空格，方便阅读
            }
        }

        // 3. 打印 ASCII 可视化部分
        fmt::print(stderr, " |");
        for (size_t j = 0; j < current_row_len; ++j) {
            unsigned char c = ptr[i + j];
            // 如果是可打印字符则显示，否则显示点号 '.'
            fmt::print(stderr, "{}", std::isprint(c) ? static_cast<char>(c) : '.');
        }
        fmt::print(stderr, "|\n");
    }
}

int main() {
    // 测试数据
    std::string message = "Hello, fmt library with C++17! This is a memory dump test.";

    fmt::print("Memory Dump of string:\n");
    hex_dump(message.data(), message.size());

    fmt::print("\nMemory Dump of a struct:\n");

    struct Test {
        int    a = 0x12345678;
        double b = 3.14159;
        char   c[4] = {'A', 'B', 'C', '\0'};
    } obj;

    hex_dump(&obj, sizeof(obj));

    return 0;
}
