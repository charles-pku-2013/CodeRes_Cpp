/*
c++ -o /tmp/test *.cc -std=c++17 -lprotobuf -g
 */
#include <iostream>
#include <sstream>
#include <string_view>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/format.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include "model_header.pb.h"

using UUID = boost::uuids::uuid;
namespace base64 = boost::beast::detail::base64;

using newtranx::ai_server::ModelHeader;

// base64 from boost libs/beast/test/beast/core/_detail_base64.cpp
namespace {
std::string base64_encode(
    std::uint8_t const* data,
    std::size_t len)
{
    std::string dest;
    dest.resize(base64::encoded_size(len));
    dest.resize(base64::encode(&dest[0], data, len));
    return dest;
}

std::string base64_encode(std::string_view s)
{
    return base64_encode (reinterpret_cast <
        std::uint8_t const*> (s.data()), s.size());
}

std::string base64_decode(std::string_view data)
{
    std::string dest;
    dest.resize(base64::decoded_size(data.size()));
    auto const result = base64::decode(
        &dest[0], data.data(), data.size());
    dest.resize(result.first);
    return dest;
}
} // namespace

std::string gen_proto() {
    ModelHeader header;

    UUID id((boost::uuids::random_generator())());
    header.set_model_id(boost::uuids::to_string(id));

    header.set_expire_date("20250701");
    header.set_n_concurrency(32);
    header.set_start_pos(256);
    header.set_end_pos(1000000);
    header.set_checksum("b76f6e8db2150098da3114da880a25aa");

    std::ostringstream oss;
    header.SerializeToOstream(&oss);

    std::string serialized_data = oss.str();
    std::cout << serialized_data.length() << std::endl;
    std::string encoded_serialized_data = base64_encode(serialized_data);
    std::cout << encoded_serialized_data.length() << std::endl;
    std::cout << encoded_serialized_data << std::endl;

    return encoded_serialized_data;
}

void decode_proto(const std::string& s) {
    std::string serialized_data = base64_decode(s);
    std::istringstream iss(serialized_data);

    ModelHeader header;
    header.ParseFromIstream(&iss);

    std::cout << header.DebugString() << std::endl;
}

int main() {
    std::string s = gen_proto();
    decode_proto(s);

    return 0;
}

