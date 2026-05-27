#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstddef>
#include <array>
#include <string>

// ==========================================
// 方案一：CRC32 校验实现 (IEEE 802.3 标准)
// ==========================================

// 编译期生成 CRC32 查询表，实现零运行时代价的查表法
constexpr std::array<uint32_t, 256> generate_crc32_table() {
    std::array<uint32_t, 256> table{};
    for (uint32_t i = 0; i < 256; ++i) {
        uint32_t crc = i;
        for (int j = 0; j < 8; ++j) {
            if (crc & 1) {
                // 0xEDB88320 是标准的 IEEE 802.3 CRC32 多项式反转值
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
        table[i] = crc;
    }
    return table;
}

/**
 * @brief 计算指定内存区域的 CRC32 校验值
 * @param data   内存起始地址
 * @param length 内存长度（字节）
 * @return 32位无符号校验和
 */
uint32_t calculate_crc32(const void* data, size_t length) {
    // static constexpr 保证这个表只在编译期生成一次
    static constexpr auto crc_table = generate_crc32_table();
    
    // 强制转换为单字节无符号指针遍历内存
    const unsigned char* p = static_cast<const unsigned char*>(data);
    
    // CRC32 初始值通常设定为 0xFFFFFFFF
    uint32_t crc = 0xFFFFFFFF;
    
    for (size_t i = 0; i < length; ++i) {
        // 查表并更新 CRC 值
        crc = (crc >> 8) ^ crc_table[(crc ^ p[i]) & 0xFF];
    }
    
    // 最后需要将结果按位取反
    return crc ^ 0xFFFFFFFF;
}

// ==========================================
// 方案二：FNV-1a Hash 实现 (极其轻量、快速)
// ==========================================

/**
 * @brief 计算指定内存区域的 FNV-1a Hash 值
 * @param data   内存起始地址
 * @param length 内存长度（字节）
 * @return 32位无符号 Hash 值
 */
uint32_t calculate_fnv1a(const void* data, size_t length) {
    const unsigned char* p = static_cast<const unsigned char*>(data);
    
    // FNV-1a 的初始偏移量 (Offset basis)
    uint32_t hash = 2166136261u;
    // FNV 质数 (FNV prime)
    const uint32_t prime = 16777619u;
    
    for (size_t i = 0; i < length; ++i) {
        hash ^= p[i];    // 先异或数据
        hash *= prime;   // 再乘以质数
    }
    
    return hash;
}

// chatgpt
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

uint32_t crc32_memory(const void* addr, std::size_t len) {
    if (addr == nullptr && len != 0) {
        // 你也可以改成 throw std::invalid_argument
        return 0;
    }

    const auto* data = static_cast<const unsigned char*>(addr);

    uint32_t crc = 0xFFFFFFFFu;

    for (std::size_t i = 0; i < len; ++i) {
        crc ^= static_cast<uint32_t>(data[i]);

        for (int bit = 0; bit < 8; ++bit) {
            if (crc & 1u) {
                crc = (crc >> 1) ^ 0xEDB88320u;
            } else {
                crc >>= 1;
            }
        }
    }

    return ~crc;
}

// === 测试用例 ===
struct TestStruct {
    int id;
    double value;
};

int main() {
    std::string text = "Hello CRC32!";
    
    std::cout << "--- 1. 字符串校验 ---" << std::endl;
    // 打印格式化为 8 位十六进制，便于核对校验码
    std::cout << "Text       : " << text << std::endl;
    std::cout << "CRC32      : 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') 
              << calculate_crc32(text.data(), text.size()) << std::endl;
    std::cout << "FNV-1a     : 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') 
              << calculate_fnv1a(text.data(), text.size()) << std::endl;

    std::cout << "\n--- 2. 结构体内存校验 ---" << std::endl;
    TestStruct ts = { 42, 3.1415 };
    std::cout << "CRC32      : 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') 
              << calculate_crc32(&ts, sizeof(ts)) << std::endl;

    // 修改结构体内存的一个字节，验证校验值是否发生剧烈变化 (雪崩效应)
    ts.id = 43;
    std::cout << "After Mod  : 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') 
              << calculate_crc32(&ts, sizeof(ts)) << std::endl;

    return 0;
}
