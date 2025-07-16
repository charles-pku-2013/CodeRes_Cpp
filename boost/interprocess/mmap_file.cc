/*
c++ -o /tmp/test test.cc -std=c++17 -lboost_iostreams
 */
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/stream.hpp>
#include <iostream>
#include <vector>

// struct mapped_file_params {
//     explicit mapped_file_params();
//     explicit mapped_file_params(const std::string& path);
//     std::string              path;
//     mapped_file::mapmode     flags;
//     std::ios_base::openmode  mode;  // Deprecated
//     stream_offset            offset;
//     std::size_t              length;
//     stream_offset            new_file_size;
//     const char*              hint;
// };

namespace bio = boost::iostreams;

int main() {
    using namespace std;

    vector<string> strArray(2000000);

    bio::mapped_file_params params("/tmp/test.cc");
    params.offset        = 10;
    params.length        = 20;  // map only first 20 chars
    params.flags         = bio::mapped_file::mapmode::readwrite;

    bio::stream<bio::mapped_file_sink> out(params);

    out.write("hello", 6);
    // out << "hello";
    out.close();

    return 0;
}
