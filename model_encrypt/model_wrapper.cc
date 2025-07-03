/*
apt install -y libpstreams-dev libboost-all-dev libgflags-dev
c++ -o model_wrapper model_wrapper.cc model_header.pb.cc -std=c++17 -lprotobuf -lgflags -g
Example:
encrypt:
./model_wrapper -e -i /tmp/model.gguf -o /tmp/enc_model.gguf -key id_rsa.pub.pem -start 256 -model_id "16:76:03:59:9e:ce" -expire_date 20250801 -model_concurrency 32
decrypt:
./model_wrapper -d -i /tmp/enc_model.gguf -o /tmp/model1.gguf -key id_rsa.pem -start 256
print header:
./model_wrapper -print_header -i /tmp/enc_model.gguf -start 256 -key id_rsa.pem
 */
#include <gflags/gflags.h>
#include <pstreams/pstream.h>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <string_view>

#include "model_header.pb.h"

using UUID = boost::uuids::uuid;
namespace fs = std::filesystem;

using newtranx::ai_server::ModelHeader;

DEFINE_string(i, "", "input file path");
DEFINE_string(o, "", "output file path");
DEFINE_bool(e, false, "wrap (encrypt) the model");
DEFINE_bool(d, false, "unwrap (decrypt) the model");
DEFINE_bool(print_header, false, "just print the header content of an encrypted model file");
DEFINE_string(model_id, "", "set model id, empty for using auto generated uuid");
DEFINE_string(expire_date, "", "set model expire date");
DEFINE_uint32(model_concurrency, 0, "set num of model concurrency");
DEFINE_uint64(start, 0, "set start pos of original model data, i.e. model header length");
DEFINE_string(key, "", "key file used for encryption or decryption");
DEFINE_uint64(min_tail_len, 128, "用于混淆的随机数据文件尾最小长度");
DEFINE_uint64(max_tail_len, 1024, "用于混淆的随机数据文件尾最大长度");

class ModelWrapper final {
 public:
    ModelWrapper();

    /**
     * @brief 对原始模型进行封装和加密
     *        从in_file_读入原始模型,封装后模型存放在out_file_
     *        封装后模型文件格式如下:
     *            header: 长度由`-start`参数指定, 存储的数据是ModelHeader类序列化后先进行加密再做base64编码
     *            body: 原始模型数据
     *            tail: 随机混淆数据,长度由`-min_tail_len`和`-max_tail_len`参数指定
     */
    void encode();

    /**
     * @brief 对加密模型进行解密
     *        从in_file_读入加密模型文件, 解密出原始模型存放于out_file_
     */
    void decode();

    /**
     * @brief 解析加密模型文件头部并打印出来
     */
    void parse_header();

 private:
    /**
     * @brief              Run system command
     *
     * @param[in]  cmd     command string like "ls -l"
     * @param[out] out     content of stdout of the command
     * @param[out] err     content of stderr of the command
     * @param[in]  input   content of stdin of the command
     *
     * @return             exit code of the command
     */
    static int sys_cmd(const std::string &cmd, std::string *out = nullptr,
                               std::string *err = nullptr, const std::string &input = "");

    // get md5sum of a file
    static std::string get_md5sum(const std::string &file);

    /**
     * @brief  encrypt / decrypt a string with openssl and provided rsa key
     *
     * @param plain / encrypted    text to be encrypted
     * @param key_file             rsa key file for encryption / decryption
     *
     * @return            encrypted / decrypted data
     */
    static std::string encrypt(const std::string &plain, const std::string &key_file);
    static std::string decrypt(const std::string &encrypted, const std::string &key_file);

    // base64 encode / decode
    static std::string base64_encode(const std::string &plain);
    static std::string base64_decode(const std::string &encoded);

    std::string                  in_file_, out_file_, key_file_;
    std::unique_ptr<ModelHeader> model_header_;
    std::string                  model_id_;
    std::string                  model_expire_date_;
    uint32_t                     model_concurrency_;
    uint64_t                     model_start_ = 0;
    uint64_t                     min_tail_len_ = 0, max_tail_len_ = 0;
};

ModelWrapper::ModelWrapper() {
    /* parse args in constructor */

    // input and output file
    in_file_ = FLAGS_i;
    out_file_ = FLAGS_o;
    key_file_ = FLAGS_key;

    if (in_file_.empty()) {
        throw std::runtime_error("input file `-i` must be specified!");
    }

    // if (out_file_.empty()) {
        // throw std::runtime_error("output file `-o` must be specified!");
    // }

    if (key_file_.empty()) {
        throw std::runtime_error("key file `-key` must be specified!");
    }

    if (!fs::exists(key_file_)) {
        throw std::runtime_error(
            boost::str(boost::format("Specified key file `%s` does not exist!") % key_file_));
    }

    // parse model header args
    model_id_ = FLAGS_model_id;
    model_expire_date_ = FLAGS_expire_date;
    model_concurrency_ = FLAGS_model_concurrency;
    model_start_ = FLAGS_start;

    if (model_start_ == 0) {
        throw std::runtime_error("`-start` must be greater than 0");
    }

    min_tail_len_ = FLAGS_min_tail_len;
    max_tail_len_ = FLAGS_max_tail_len;
    assert(min_tail_len_ <= max_tail_len_);
}

void ModelWrapper::encode() {
    // check file
    std::ifstream ifs(in_file_, std::ios::in | std::ios::binary);
    std::ofstream ofs(out_file_, std::ios::out | std::ios::binary);

    if (!ifs) {
        throw std::runtime_error(
            boost::str(boost::format("Failed to open file `%s` for input") % in_file_));
    }
    if (!ofs) {
        throw std::runtime_error(
            boost::str(boost::format("Failed to open file `%s` for output") % out_file_));
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
        throw std::runtime_error(boost::str(boost::format("Invalid `start_pos=%lu end_pos=%lu`") %
                                            model_header_->start_pos() % model_header_->end_pos()));
    }

    std::string cksum = get_md5sum(in_file_);
    if (cksum.empty()) {
        throw std::runtime_error(
            boost::str(boost::format("Failed to get checksum of input file `%s`") % in_file_));
    }
    model_header_->set_checksum(cksum);

    // encrypt header with ssl and then encode with base64
    std::ostringstream oss;
    if (!model_header_->SerializeToOstream(&oss)) {
        throw std::runtime_error("Failed to serialize model header!");
    }
    std::string header_data = oss.str();
    std::string encrypted_header = encrypt(header_data, key_file_);
    std::string encoded_header = base64_encode(encrypted_header);

    if (encoded_header.length() >= model_start_) {
        throw std::runtime_error(
            boost::str(boost::format("Encoded header length `%lu` is larger than specified model "
                                     "start pos `%lu`, please reset the value with `-start`") %
                       (encoded_header.length() + 1) % model_start_));
    }
    encoded_header.resize(model_start_, 0);

    std::cerr << "Writting model header to output file ..." << std::endl;
    ofs.write(encoded_header.c_str(), model_start_);
    if (!ofs) {
        throw std::runtime_error("Failed to write header to output file!");
    }
    ofs.close();

    std::cerr << "Embedding original model to output file ..." << std::endl;
    std::string cmd = boost::str(boost::format("cat '%s' >> '%s'") % in_file_ % out_file_);
    std::string err;
    int         retval = sys_cmd(cmd, nullptr, &err);

    if (retval) {
        throw std::runtime_error(
            boost::str(boost::format("Failed to copy original model to output file: %s") % err));
    }

    // 在文件末尾添加混淆随机数据
    std::random_device            rd;         // obtain a random number from hardware
    std::mt19937                  gen(rd());  // seed the generator
    std::uniform_int_distribution distr(min_tail_len_, max_tail_len_);  // define the range
    int                           tail_len = distr(gen);

    cmd = boost::str(boost::format("head -c %d /dev/random >> '%s'") % tail_len % out_file_);
    retval = sys_cmd(cmd, nullptr, &err);

    if (retval) {
        std::cerr << "WARNING! Failed to add tail data to output file, err: " << err << std::endl;
    } else {
        std::cerr << boost::format("Added %d bytes random tail block to output file.") % tail_len
                  << std::endl;
    }

    std::cerr << boost::format("Successfully encoded model from `%s` to `%s`") % in_file_ %
                     out_file_
              << std::endl;
    std::cerr << model_header_->DebugString() << std::endl;
}

void ModelWrapper::decode() {
    // get model header
    parse_header();

    if (model_header_->start_pos() >= model_header_->end_pos()) {
        throw std::runtime_error(
            boost::str(boost::format("Invalid model file info `start_pos=%lu end_pos=%lu`") %
                       model_header_->start_pos() % model_header_->end_pos()));
    }

    std::cerr << boost::format("Extracting original model to %s ...") % out_file_ << std::endl;
    uint64_t    sz = model_header_->end_pos() - model_header_->start_pos();
    std::string cmd = boost::str(boost::format("tail -c +%lu '%s' | head -c %lu > '%s'") %
                                 (model_header_->start_pos() + 1) % in_file_ % sz % out_file_);

    std::string err;
    int         retval = sys_cmd(cmd, nullptr, &err);

    if (retval) {
        throw std::runtime_error(
            boost::str(boost::format("Failed to extract original model: %s") % err));
    }

    std::cerr << "Checking extracted file ..." << std::endl;
    std::string cksum = get_md5sum(out_file_);

    if (cksum != model_header_->checksum()) {
        throw std::runtime_error(
            boost::str(boost::format("Checksum comparation fail, which is %s but expected %s") %
                       cksum % model_header_->checksum()));
    }

    std::cerr << boost::format("Successfully extracted original model to %s") % out_file_
              << std::endl;
}

void ModelWrapper::parse_header() {
    std::ifstream ifs(in_file_, std::ios::in | std::ios::binary);
    if (!ifs) {
        throw std::runtime_error(
            boost::str(boost::format("Failed to open file `%s` for input") % in_file_));
    }

    // read and decrypt decode model header
    std::cerr << "Parsing model header..." << std::endl;
    std::vector<char> raw_header(model_start_, 0);
    ifs.read(raw_header.data(), model_start_);
    if (!ifs) {
        throw std::runtime_error("Failed to read model header from input file!");
    }
    ifs.close();

    std::string        encoded_header(raw_header.data());
    std::string        encrypted_header = base64_decode(encoded_header);
    std::string        plain_header = decrypt(encrypted_header, key_file_);
    std::istringstream iss(plain_header);

    model_header_.reset(new ModelHeader);
    if (!model_header_->ParseFromIstream(&iss)) {
        throw std::runtime_error("Failed to parse model header!");
    }

    std::cerr << "Successfully read model header:" << std::endl;
    std::cerr << model_header_->DebugString() << std::endl;
}

std::string ModelWrapper::get_md5sum(const std::string &file) {
    std::string cmd = boost::str(boost::format("md5sum '%s' | awk '{print $1}'") % file);
    std::string out, err;
    int         retval = sys_cmd(cmd, &out, &err);

    if (!err.empty()) {
        std::cerr << boost::format("Get md5 of file `%s` error: %s") % file % err << std::endl;
    }

    return retval == 0 ? out : "";
}

int ModelWrapper::sys_cmd(const std::string &cmd, std::string *out, std::string *err,
                          const std::string &input) {
    const redi::pstreams::pmode all3streams =
        redi::pstreams::pstdin | redi::pstreams::pstdout | redi::pstreams::pstderr;

    redi::pstream ps(cmd, all3streams);

    if (!input.empty()) {
        ps << input << redi::peof;
    }

    if (out) {
        std::stringstream ss;
        ss << ps.out().rdbuf();
        *out = ss.str();
        boost::trim(*out);
    }

    ps.clear();

    if (err) {
        std::stringstream ss;
        ss << ps.err().rdbuf();
        *err = ss.str();
        boost::trim(*err);
    }

    return ps.close();
}

std::string ModelWrapper::encrypt(const std::string &plain, const std::string &key_file) {
    std::string cmd =
        boost::str(boost::format("openssl pkeyutl -encrypt -inkey %s -pubin") % key_file);

    std::string encrypted, err;

    int status = sys_cmd(cmd, &encrypted, &err, plain);

    if (status) {
        throw std::runtime_error(boost::str(
            boost::format("OpenSSL encryption fail, cmd: `%s`, err_msg: %s") % cmd % err));
    }

    return encrypted;
}

std::string ModelWrapper::decrypt(const std::string &encrypted, const std::string &key_file) {
    std::string cmd = boost::str(boost::format("openssl pkeyutl -decrypt -inkey %s") % key_file);

    std::string plain, err;

    int status = sys_cmd(cmd, &plain, &err, encrypted);

    if (status) {
        throw std::runtime_error(boost::str(
            boost::format("OpenSSL decryption fail, cmd: `%s`, err_msg: %s") % cmd % err));
    }

    return plain;
}

std::string ModelWrapper::base64_encode(const std::string &plain) {
    std::string encoded, err;

    int status = sys_cmd("base64", &encoded, &err, plain);

    if (status) {
        throw std::runtime_error(
            boost::str(boost::format("base64 encode fail, err_msg: %s") % err));
    }

    return encoded;
}

std::string ModelWrapper::base64_decode(const std::string &encoded) {
    std::string plain, err;

    int status = sys_cmd("base64 -d", &plain, &err, encoded);

    if (status) {
        throw std::runtime_error(
            boost::str(boost::format("base64 decode fail, err_msg: %s") % err));
    }

    return plain;
}

int main(int argc, char **argv) {
    GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);

    try {
        ModelWrapper model_wrapper;

        if (FLAGS_e) {
            model_wrapper.encode();
        } else if (FLAGS_d) {
            model_wrapper.decode();
        } else if (FLAGS_print_header) {
            model_wrapper.parse_header();
        } else {
            std::cerr << "Task type `-e` or `-d` or `-print_header` must be specified!" << std::endl;
            return -1;
        }

    } catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        // remove any out file on failure
        std::error_code ec;
        fs::remove(FLAGS_o, ec);
        return -1;
    }

    return 0;
}
