#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <array>
#include <iostream>

uint32_t memory_crc32(const void* addr, std::size_t len) {
    if (!addr || !len) {
        return 0;
    }

    static constexpr auto crc32_table = []() constexpr {
        std::array<uint32_t, 256> table{};

        for (uint32_t i = 0; i < 256; ++i) {
            uint32_t crc = i;

            for (int bit = 0; bit < 8; ++bit) {
                crc = (crc & 1u)
                    ? ((crc >> 1) ^ 0xEDB88320u)
                    : (crc >> 1);
            }

            table[i] = crc;
        }

        return table;
    }();

    const auto* p = static_cast<const unsigned char*>(addr);

    uint32_t crc = 0xFFFFFFFFu;

    for (std::size_t i = 0; i < len; ++i) {
        uint8_t idx = static_cast<uint8_t>((crc ^ p[i]) & 0xFFu);
        crc = (crc >> 8) ^ crc32_table[idx];
    }

    return ~crc;
}

std::string memory_crc32_string(const void* addr, std::size_t len) {
    std::ostringstream oss;

    oss << '\n';

    if (addr == nullptr && len != 0) {
        oss << "CRC32: <invalid null pointer, len=" << len << ">";
        oss << '\n';
        return oss.str();
    }

    uint32_t crc = memory_crc32(addr, len);

    oss << "addr : " << addr << '\n';
    oss << "len  : " << len << " bytes\n";
    oss << "CRC32: 0x"
        << std::hex
        << std::uppercase
        << std::setw(8)
        << std::setfill('0')
        << crc
        << '\n';

    oss << '\n';

    return oss.str();
}

int main() {
    std::string s = "hello";

    std::cout << memory_crc32_string(s.data(), s.size());

    int value = 0x12345678;
    std::cout << memory_crc32_string(&value, sizeof(value));

    return 0;
}
