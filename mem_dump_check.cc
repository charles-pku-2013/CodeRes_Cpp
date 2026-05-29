#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <cctype>
#include <iostream>

std::string dump_memory(const void* addr,
                        std::size_t len,
                        std::size_t bytes_per_line = 16) {
    std::ostringstream oss;

    // 要求：前面有换行符
    oss << '\n';

    if (addr == nullptr) {
        oss << "<null pointer>";
        oss << '\n';
        return oss.str();
    }

    if (bytes_per_line == 0) {
        bytes_per_line = 16;
    }

    const auto* p = static_cast<const unsigned char*>(addr);

    for (std::size_t offset = 0; offset < len; offset += bytes_per_line) {
        const std::size_t line_len =
            (offset + bytes_per_line <= len) ? bytes_per_line : (len - offset);

        // 地址列，类似 gdb/hexdump
        oss << "0x"
            << std::hex
            << std::setw(sizeof(void*) * 2)
            << std::setfill('0')
            << reinterpret_cast<std::uintptr_t>(p + offset)
            << ": ";

        // 十六进制字节列
        for (std::size_t i = 0; i < bytes_per_line; ++i) {
            if (i < line_len) {
                oss << std::setw(2)
                    << std::setfill('0')
                    << static_cast<unsigned int>(p[offset + i])
                    << ' ';
            } else {
                oss << "   ";
            }

            // 8 字节后加一个空格，视觉上像小河分叉
            if (i == 7) {
                oss << ' ';
            }
        }

        // ASCII 列
        oss << " |";
        for (std::size_t i = 0; i < line_len; ++i) {
            unsigned char c = p[offset + i];
            oss << (std::isprint(c) ? static_cast<char>(c) : '.');
        }
        oss << '|';

        oss << '\n';
    }

    // 要求：后面也有换行符。
    // 上面的循环每行已经输出 '\n'，但 len == 0 时还需要补一个。
    if (len == 0) {
        oss << '\n';
    }

    return oss.str();
}

int main() {
    std::string s = "Hello, gdb-style memory dump!\n";

    std::cout << dump_memory(s.data(), s.size());

    int x = 0x12345678;
    std::cout << dump_memory(&x, sizeof(x));

    return 0;
}
