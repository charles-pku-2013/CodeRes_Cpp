#include <boost/crc.hpp>
#include <boost/format.hpp>
#include <string>
#include <iostream>

using namespace std;

namespace {
uint32_t checksum(const void *data, std::size_t length) {
    boost::crc_32_type result;
    result.process_bytes(data, length);
    return result.checksum();
}
uint32_t checksum(const std::string &str)
{ return checksum(str.data(), str.length()); }
}  // namespace

uint32_t GetCrc32(const string& my_string)
{
    boost::crc_32_type result;
    result.process_bytes(my_string.data(), my_string.length());
    return result.checksum();
}


uint64_t get_id(int iType, int iDetailType, const std::string &strPhrase)
{
    uint64_t ret = 0;
    ret += (uint64_t)iType << 48;
    ret += (uint64_t)iDetailType << 32;

    boost::crc_32_type crcChecker;
    crcChecker.process_bytes(strPhrase.data(), strPhrase.length());
    ret += crcChecker.checksum();

    return ret;
}

int main()
{
    // string str;
    // while (getline(cin, str)) {
        // cout << boost::format("%08x") % GetCrc32(str) << endl;
    // } // while

    cout << boost::format("%08x") % GetCrc32("烤鸭") << endl;
    cout << boost::format("%016x") % get_id(1, 3, "烤鸭") << endl;

    return 0;
}


