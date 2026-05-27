#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstdint>
#include <string>

/**
 * @brief 打印指定内存区域的内容，格式类似于 GDB 的 x 命令
 * * @param address 内存起始地址
 * @param size    需要读取的字节总长度
 */
void mem_dump(const void* address, size_t size) {
    // 将地址转换为单字节无符号指针，确保每次只读取一个字节且无符号扩展问题
    const unsigned char* p = static_cast<const unsigned char*>(address);
    
    // 每行打印 16 个字节
    const size_t columns = 16; 

    // 保存 std::cout 的原始格式状态，避免污染后续输出
    std::ios_base::fmtflags old_flags = std::cout.flags();
    char old_fill = std::cout.fill();

    for (size_t i = 0; i < size; i += columns) {
        // 1. 打印内存地址 (使用 uintptr_t 保证跨平台指针大小兼容)
        std::cout << "0x" 
                  << std::hex << std::setw(sizeof(void*) * 2) << std::setfill('0')
                  << reinterpret_cast<uintptr_t>(p + i) << ":  ";

        // 2. 打印十六进制数据
        for (size_t j = 0; j < columns; ++j) {
            if (i + j < size) {
                // 打印两位的十六进制数值
                std::cout << std::hex << std::setw(2) << std::setfill('0') 
                          << static_cast<int>(p[i + j]) << " ";
            } else {
                // 如果最后一行不足 16 字节，用空格补齐以对齐右侧的 ASCII 栏
                std::cout << "   "; 
            }
            
            // 在第 8 个字节后加一个额外空格，增加可读性（GDB 默认风格）
            if (j == 7) {
                std::cout << " ";
            }
        }

        std::cout << "  "; // 十六进制和 ASCII 之间的分隔

        // 3. 打印对应的 ASCII 字符
        for (size_t j = 0; j < columns && (i + j) < size; ++j) {
            unsigned char c = p[i + j];
            // std::isprint 检查是否为可打印字符 (ASCII 32~126)
            if (std::isprint(c)) {
                std::cout << static_cast<char>(c);
            } else {
                std::cout << '.'; // 不可打印字符替换为 '.'
            }
        }
        std::cout << '\n';
    }

    // 恢复 std::cout 的原始状态
    std::cout.flags(old_flags);
    std::cout.fill(old_fill);
}

// === 测试用例 ===
struct TestStruct {
    int id;
    double value;
    char name[12];
};

int main() {
    std::cout << "--- 1. 查看字符串的内存 ---" << std::endl;
    std::string text = "Hello GDB Memory Dump!\n";
    // 注意：如果是 std::string，我们需要查看 data() 指向的堆内存，而不是 std::string 对象本身
    mem_dump(text.data(), text.size() + 1); // +1 包含 \0 结尾
    
    std::cout << "\n--- 2. 查看结构体的内存 ---" << std::endl;
    TestStruct ts = { 2026, 3.14159, "C++ Core" };
    mem_dump(&ts, sizeof(TestStruct));

    std::cout << "\n--- 3. 查看整型数组的内存 ---" << std::endl;
    uint32_t arr[] = { 0x11223344, 0x55667788, 0x99AABBCC };
    mem_dump(arr, sizeof(arr));

    return 0;
}
