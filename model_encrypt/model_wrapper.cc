/*
c++ -o /tmp/test model_wrapper.cc model_header.pb.cc -std=c++17 -lprotobuf -lgflags -lboost_iostreams -g
Example:
encrypt:
./model_wrapper -e -i /tmp/model.gguf -o /tmp/enc_model.gguf -start 256 -expire_date 20250801 -model_concurrency 32
decrypt:
./model_wrapper -d -i /tmp/enc_model.gguf -o /tmp/model.gguf -start 256
 */
#include <gflags/gflags.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <string_view>
#include <sys/types.h>
#include <sys/wait.h>
#include <boost/algorithm/string.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/format.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include "model_header.pb.h"

using UUID = boost::uuids::uuid;
namespace base64 = boost::beast::detail::base64;
namespace fs = std::filesystem;

using newtranx::ai_server::ModelHeader;

DEFINE_string(i, "", "input file path");
DEFINE_string(o, "", "output file path");
DEFINE_bool(e, false, "wrap (encrypt) the model");
DEFINE_bool(d, false, "unwrap (decrypt) the model");
DEFINE_string(model_id, "", "set model id, empty for using auto generated uuid");
DEFINE_string(expire_date, "", "set model expire date");
DEFINE_uint32(model_concurrency, 0, "set num of model concurrency");
DEFINE_uint64(start, 0, "set start pos of original model data, i.e. model header length");

class ModelWrapper final {
public:
    ModelWrapper();

    void encode();
    void decode();

private:
    static int sysCmd(const std::string &cmd, std::string *out);
    static std::string getMD5(const std::string& file);

    // base64 from boost libs/beast/test/beast/core/_detail_base64.cpp
    static std::string base64_encode(std::uint8_t const* data, std::size_t len);
    static std::string base64_encode(std::string_view s);
    static std::string base64_decode(std::string_view data);

    std::string                     in_file_, out_file_;
    std::unique_ptr<ModelHeader>    model_header_;
    std::string                     model_id_;
    std::string                     model_expire_date_;
    uint32_t                        model_concurrency_;
    uint64_t                        model_start_ = 0;
};

ModelWrapper::ModelWrapper() {
    /* parse args in constructor */

    // input and output file
    in_file_ = FLAGS_i;
    out_file_ = FLAGS_o;

    if (in_file_.empty()) {
        throw std::runtime_error("input file `-i` must be specified!");
    }

    if (out_file_.empty()) {
        throw std::runtime_error("output file `-o` must be specified!");
    }

    // parse model header args
    model_id_ = FLAGS_model_id;
    model_expire_date_ = FLAGS_expire_date;
    model_concurrency_ = FLAGS_model_concurrency;
    model_start_ = FLAGS_start;

    if (model_start_ == 0) {
        throw std::runtime_error("`-start` must be greater than 0");
    }
}

void ModelWrapper::encode() {
    // check file
    std::ifstream ifs(in_file_, std::ios::in | std::ios::binary);
    std::ofstream ofs(out_file_, std::ios::out | std::ios::binary);

    if (!ifs) {
        throw std::runtime_error(boost::str(boost::format("Failed to open file `%s` for input") % in_file_));
    }
    if (!ofs) {
        throw std::runtime_error(boost::str(boost::format("Failed to open file `%s` for output") % out_file_));
    }
    ifs.close();

    std::cerr << "Generating model header..." << std::endl;
    model_header_.reset(new ModelHeader);

    if (model_id_.empty()) {
        model_header_->set_model_id(boost::uuids::to_string(boost::uuids::random_generator()()));
    } else {
        model_header_->set_model_id(model_id_);
    }

    model_header_->set_expire_date(model_expire_date_);

    model_header_->set_n_concurrency(model_concurrency_);

    model_header_->set_start_pos(model_start_);
    uint64_t in_file_sz = fs::file_size(in_file_);
    model_header_->set_end_pos(model_start_ + in_file_sz);

    if (model_header_->start_pos() >= model_header_->end_pos()) {
        throw std::runtime_error(boost::str(
            boost::format("Invalid `start_pos=%lu end_pos=%lu`")
            % model_header_->start_pos() % model_header_->end_pos()));
    }

    std::string cksum = getMD5(in_file_);
    if (cksum.empty()) {
        throw std::runtime_error(boost::str(boost::format("Failed to get checksum of input file `%s`") % in_file_));
    }
    model_header_->set_checksum(cksum);

    // encode header with base64
    std::ostringstream oss;
    if (!model_header_->SerializeToOstream(&oss)) {
        throw std::runtime_error("Failed to serialize model header!");
    }
    std::string header_data = oss.str();
    std::string encoded_header = base64_encode(header_data);

    if (encoded_header.length() >= model_start_) {
        throw std::runtime_error(boost::str(boost::format(
            "Encoded header length `%lu` is larger than specified model start pos `%lu`, please reset the value with `-start`"
        ) % (encoded_header.length() + 1) % model_start_));
    }
    encoded_header.resize(model_start_, 0);

    std::cerr << "Writting model header to output file ..." << std::endl;
    ofs.write(encoded_header.c_str(), model_start_);
    if (!ofs) {
        throw std::runtime_error("Failed to write header to output file!");
    }
    ofs.close();

    std::cerr << "Copying original model to output file ..." << std::endl;
    std::string cmd = boost::str(boost::format( "cat '%s' >> '%s'") % in_file_ % out_file_);
    int retval = sysCmd(cmd, nullptr);

    if (retval) {
        throw std::runtime_error("Failed to copy original model to output file!");
    }

    std::cerr << boost::format("Successfully encoded model from `%s` to `%s`") % in_file_ % out_file_ << std::endl;
    std::cerr << model_header_->DebugString() << std::endl;
}

void ModelWrapper::decode() {
    std::ifstream ifs(in_file_, std::ios::in | std::ios::binary);
    if (!ifs) {
        throw std::runtime_error(boost::str(boost::format("Failed to open file `%s` for input") % in_file_));
    }

    // read and decode model header
    std::vector<char> raw_header(model_start_, 0);
    ifs.read(raw_header.data(), model_start_);
    if (!ifs) {
        throw std::runtime_error("Failed to read model header from input file!");
    }
    ifs.close();

    std::string encoded_header(raw_header.data());
    std::string decoded_header = base64_decode(encoded_header);
    std::istringstream iss(decoded_header);

    model_header_.reset(new ModelHeader);
    if (!model_header_->ParseFromIstream(&iss)) {
        throw std::runtime_error("Failed to parse model header!");
    }

    std::cerr << "Successfully read model header:" << std::endl;
    std::cerr << model_header_->DebugString() << std::endl;

    if (model_header_->start_pos() >= model_header_->end_pos()) {
        throw std::runtime_error(boost::str(boost::format("Invalid model file info `start_pos=%lu end_pos=%lu`")
                % model_header_->start_pos() % model_header_->end_pos()));
    }

    std::cerr << boost::format("Extracting original model to %s ...") % out_file_ << std::endl;
    uint64_t sz = model_header_->end_pos() - model_header_->start_pos();
    std::string cmd = boost::str(boost::format(
        "tail -c +%lu '%s' | head -c %lu > '%s'"
    ) % (model_header_->start_pos() + 1) % in_file_ % sz % out_file_);

    int retval = sysCmd(cmd, nullptr);
    if (retval) {
        throw std::runtime_error("Failed to extract original model!");
    }

    std::cerr << "Checking extracted file ..." << std::endl;
    std::string cksum = getMD5(out_file_);
    if (cksum != model_header_->checksum()) {
        throw std::runtime_error(boost::str(
              boost::format("Checksum comparation fail, which is %s but expected %s")
              % cksum % model_header_->checksum()));
    }

    std::cerr << boost::format("Successfully extracted original model to %s") % out_file_ << std::endl;
}

std::string ModelWrapper::getMD5(const std::string& file) {
    std::string cmd = boost::str(boost::format("md5sum '%s' | awk '{print $1}'") % file);
    std::string out;
    int retval = sysCmd(cmd, &out);
    return retval == 0 ? out : "";
}

int ModelWrapper::sysCmd(const std::string &cmd, std::string *out) {
    int retval = -1;

    std::unique_ptr<FILE, std::function<void(FILE*)>> pp(
        ::popen(cmd.c_str(), "r"),
        [&retval](FILE* p) {
            if(p) {
                retval = ::pclose(p);
                retval = WEXITSTATUS(retval);
            }
        }
    );

    if (!pp) {
        if (out) {
            *out = "popen error!";
        }
        return -1;
    }
    ::setlinebuf(pp.get());

    using PipeStream = boost::iostreams::stream<boost::iostreams::file_descriptor_source>;
    PipeStream stream(fileno(pp.get()), boost::iostreams::never_close_handle);

    std::stringstream ss;
    ss << stream.rdbuf();

    if (out) {
        *out = ss.str();
        boost::trim(*out);
    }

    pp.reset();
    return retval;
}

std::string ModelWrapper::base64_encode(std::uint8_t const* data, std::size_t len) {
    std::string dest;
    dest.resize(base64::encoded_size(len));
    dest.resize(base64::encode(&dest[0], data, len));
    return dest;
}

std::string ModelWrapper::base64_encode(std::string_view s) {
    return base64_encode(reinterpret_cast<std::uint8_t const*>(s.data()), s.size());
}

std::string ModelWrapper::base64_decode(std::string_view data) {
    std::string dest;
    dest.resize(base64::decoded_size(data.size()));
    auto const result = base64::decode(
        &dest[0], data.data(), data.size());
    dest.resize(result.first);
    return dest;
}

int main(int argc, char **argv) {
    GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);

    try {
        ModelWrapper model_wrapper;

        if (FLAGS_e) {
            model_wrapper.encode();
        } else if (FLAGS_d) {
            model_wrapper.decode();
        } else {
            std::cerr << "Task type `-e` or `-d` must be specified!" << std::endl;
            return -1;
        }

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        // remove any out file on failure
        std::error_code ec;
        fs::remove(FLAGS_o, ec);
        return -1;
    }

    return 0;
}
