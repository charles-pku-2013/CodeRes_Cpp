#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstdint>
#include <string>
#include <sstream>

/**
 * @brief 格式化指定内存区域的内容为字符串，格式类似于 GDB 的 x 命令
 * @param address 内存起始地址
 * @param size    需要读取的字节总长度
 * @return 包含格式化内存数据的字符串
 */
std::string examine_memory(const void* address, size_t size) {
    const unsigned char* p = static_cast<const unsigned char*>(address);
    const size_t columns = 16; 
    
    // 使用 ostringstream 来专门构建输出字符串
    std::ostringstream oss;

    // 按照需求，在字符串开头增加换行符，防止调用方的同行打印破坏首行对齐
    oss << '\n';

    for (size_t i = 0; i < size; i += columns) {
        // 1. 打印内存地址
        oss << "0x" 
            << std::hex << std::setw(sizeof(void*) * 2) << std::setfill('0')
            << reinterpret_cast<uintptr_t>(p + i) << ":  ";

        // 2. 打印十六进制数据
        for (size_t j = 0; j < columns; ++j) {
            if (i + j < size) {
                // 强制转为 int 打印，同时 setw(2) 保证对齐
                oss << std::hex << std::setw(2) << std::setfill('0') 
                    << static_cast<int>(p[i + j]) << " ";
            } else {
                // 最后一行不足 16 字节，用空格补齐以对齐右侧的 ASCII 栏
                oss << "   "; 
            }
            
            // 在第 8 个字节后加一个额外空格，增加可读性
            if (j == 7) {
                oss << " ";
            }
        }

        oss << "  "; // 十六进制和 ASCII 之间的分隔

        // 3. 打印对应的 ASCII 字符
        for (size_t j = 0; j < columns && (i + j) < size; ++j) {
            unsigned char c = p[i + j];
            if (std::isprint(c)) {
                oss << static_cast<char>(c);
            } else {
                oss << '.'; // 不可打印字符替换为 '.'
            }
        }
        
        // 每行结束添加换行
        oss << '\n';
    }

    // 将 stringstream 内部的缓冲转化为 string 并返回
    return oss.str();
}

// chatgpt
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <cctype>

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

// === 测试用例 ===
struct TestStruct {
    int id;
    double value;
};

int main() {
    std::string text = "Hello GDB Memory Dump!";
    
    // 测试：将自定义前缀和内存 dump 在同一条语句中输出
    // 即使在同一行打印，得益于返回的字符串首字符是 '\n'，内存地址依然会完美对齐
    std::cout << "--- 1. String Dump ---" 
              << examine_memory(text.data(), text.size());
    
    TestStruct ts = { 2026, 3.14159 };
    std::cout << "\n--- 2. Struct Dump ---" 
              << examine_memory(&ts, sizeof(TestStruct));

    return 0;
}
