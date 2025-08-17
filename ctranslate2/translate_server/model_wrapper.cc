#include "model_wrapper.h"

#include <sstream>
#include <fstream>
#include <filesystem>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <pstreams/pstream.h>
#include <glog/logging.h>

namespace newtranx {
namespace ai_server {

namespace fs = std::filesystem;

void ModelWrapper::mount_fs(std::size_t size) {
    // create mount point
    int status = sys_cmd("mktemp -d", &mount_path_);
    if (status) {
        mount_path_.clear();
        throw std::runtime_error(boost::str(boost::format("[%s:%d] Parse model fail!") % __FILE__ % __LINE__));
    }

    // create ramfs on mount point
    std::string cmd = boost::str(boost::format("mount -o size=%lu -t tmpfs none %s") % size % mount_path_);
    status = sys_cmd(cmd);
    if (status) {
        umount_fs();
        throw std::runtime_error(boost::str(boost::format("[%s:%d] Parse model fail!") % __FILE__ % __LINE__));
    }

    DLOG(INFO) << "Successfully created ramfs " << mount_path_;
}

void ModelWrapper::umount_fs() {
    if (!mount_path_.empty()) {
        // perform 'lazy' umount
        std::string cmd = boost::str(boost::format("umount -l %s") % mount_path_);
        sys_cmd(cmd);
        // remove mount dir
        std::error_code ec;
        fs::remove_all(mount_path_, ec);
        mount_path_.clear();
    }
}

void ModelWrapper::parse_model_header(const std::string& model_path) {
    constexpr int MAX_HEADER_LEN = 4096;

    // decrypt private_key with password 'passwd_' from data 'encrypted_key_'
    std::string private_key;
    {
        std::string cmd = boost::str(boost::format("openssl aes-256-cbc -d -pbkdf2 -pass pass:%s") % passwd_);

        std::stringstream ss;
        ss.write(reinterpret_cast<const char*>(&encrypted_key_[0]), std::size(encrypted_key_)).flush();

        int status = sys_cmd_in_stream(cmd, &private_key, nullptr, &ss);

        if (status) {
            throw std::runtime_error(boost::str(boost::format("[%s:%d] Parse model fail!") % __FILE__ % __LINE__));
        }
    }

    // write private key to tmp file
    std::string private_key_file = sys_cmd_simple(
        boost::str(boost::format("mktemp -u -p %s") % mount_path_)
    );

    if (private_key_file.empty()) {
        throw std::runtime_error(boost::str(boost::format("[%s:%d] Parse model fail!") % __FILE__ % __LINE__));
    }

    // remove private key file immediatelly when function ends
    std::unique_ptr<void, std::function<void(void*)>> auto_remove_key_file(
        (void*)1,
        [&private_key_file](void*) {
            std::error_code ec;
            fs::remove(private_key_file, ec);
        }
    );

    // write private key to file
    std::ofstream ofs(private_key_file, std::ios::out);
    if (!ofs) {
        throw std::runtime_error(boost::str(boost::format("[%s:%d] Parse model fail!") % __FILE__ % __LINE__));
    }
    ofs << private_key << std::flush;
    ofs.close();

    // read and decrypt model header
    std::ifstream ifs(model_path, std::ios::in | std::ios::binary);
    if (!ifs) {
        throw std::runtime_error(boost::str(boost::format("[%s:%d] Parse model fail!") % __FILE__ % __LINE__));
    }

    std::vector<char> raw_header(MAX_HEADER_LEN, 0);
    ifs.read(raw_header.data(), MAX_HEADER_LEN);
    if (ifs.gcount() == 0) {
        throw std::runtime_error(boost::str(boost::format("[%s:%d] Parse model fail!") % __FILE__ % __LINE__));
    }
    ifs.close();

    std::string        encoded_header(raw_header.data());
    std::string        encrypted_header = base64_decode(encoded_header);
    std::string        plain_header = decrypt_text(encrypted_header, private_key_file);
    auto_remove_key_file.reset();  // remove private key immediatelly

    std::istringstream iss(plain_header);
    model_header_.reset(new ModelHeader);
    if (!model_header_->ParseFromIstream(&iss)) {
        throw std::runtime_error(boost::str(boost::format("[%s:%d] Parse model fail!") % __FILE__ % __LINE__));
    }

    DLOG(INFO) << "Model header: " << model_header_->DebugString();
}

std::tuple<std::string, std::string> ModelWrapper::decrypt_model(const std::string& encrypted_model_path) {
    // mount ramdisk
    std::size_t sz = fs::file_size(encrypted_model_path);

    if(!sz) {
        throw std::runtime_error(boost::str(boost::format("[%s:%d] Parse model fail!") % __FILE__ % __LINE__));
    }

    sz += 10 * 1024 * 1024;  // add extra 1mb for redundancy
    mount_fs(sz);

    if (mount_path_.empty()) {
        throw std::runtime_error("mount fs fail!");
    }

    // read model_header_
    parse_model_header(encrypted_model_path);

    if (!model_header_->is_directory()) {
        throw std::runtime_error("Invalid model! Model must be a directory!");
    }
    if (model_header_->start_pos() >= model_header_->end_pos()) {
        throw std::runtime_error(boost::str(boost::format("[%s:%d] Parse model fail!") % __FILE__ % __LINE__));
    }

    sz = model_header_->end_pos() - model_header_->start_pos();
    std::string cmd = boost::str(
        boost::format("tail -c +%lu '%s' | head -c %lu | tee '._tmp_model.tar' | md5sum | awk '{print $1}' && "
                      "tar -xf '._tmp_model.tar' -C '%s' --strip-components 1; rm -f ._tmp_model.tar") %
        (model_header_->start_pos() + 1) % encrypted_model_path % sz % mount_path_);

    std::string cksum, err;
    int         retval = sys_cmd(cmd, &cksum, &err);

    if (retval) {
        throw std::runtime_error(
            boost::str(boost::format("Failed to extract original model. cmd: '%s', err: '%s'") % cmd % err));
    }

    if (cksum != model_header_->checksum()) {
        throw std::runtime_error(boost::str(boost::format("Checksum comparation fail, which is %s but expected %s") %
                                            cksum % model_header_->checksum()));
    }

    DLOG(INFO) << "Successfully extracted model!";

    std::string s_model = (fs::path(mount_path_) / fs::path("spm.model")).string();

    if (!fs::exists(s_model)) {
        throw std::runtime_error("No sentencepiece model found in model pack!");
    }

    return std::make_tuple(mount_path_, s_model);
}

std::string ModelWrapper::base64_decode(const std::string &encoded) {
    std::string plain, err;

    int status = sys_cmd("base64 -d", &plain, &err, encoded);

    if (status) {
        throw std::runtime_error(boost::str(boost::format("[%s:%d] Parse model fail!") % __FILE__ % __LINE__));
    }

    return plain;
}

std::string ModelWrapper::decrypt_text(const std::string &encrypted, const std::string &key_file) {
    std::string cmd = boost::str(boost::format("openssl pkeyutl -decrypt -inkey %s") % key_file);

    std::string plain, err;

    int status = sys_cmd(cmd, &plain, &err, encrypted);

    if (status) {
        throw std::runtime_error(boost::str(boost::format("[%s:%d] Parse model fail!") % __FILE__ % __LINE__));
    }

    return plain;
}

int ModelWrapper::sys_cmd(const std::string &cmd, std::string *out, std::string *err,
            const std::string &input) {
    std::istream* in_stream = nullptr;

    std::stringstream ss;
    if (!input.empty()) {
        ss << input << std::flush;
        in_stream = &ss;
    }

    return sys_cmd_in_stream(cmd, out, err, in_stream);
}

int ModelWrapper::sys_cmd_in_stream(const std::string &cmd, std::string *out, std::string *err,
                      std::istream* in_stream) {
    const redi::pstreams::pmode all3streams =
        redi::pstreams::pstdin | redi::pstreams::pstdout | redi::pstreams::pstderr;

    redi::pstream ps(cmd, all3streams);

    if (in_stream) {
        ps << in_stream->rdbuf() << redi::peof;
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

std::string ModelWrapper::sys_cmd_simple(const std::string &cmd, const std::string &input) {
    std::string out, err;

    int retval = sys_cmd(cmd, &out, &err, input);

    if (retval) {
        throw std::runtime_error(boost::str(boost::format(
            "Failed to run command '%s', error: '%s'"
        ) % cmd % err));
    }

    return out;
}

}  // namespace ai_server
}  // namespace newtranx

