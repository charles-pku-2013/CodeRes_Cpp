#pragma once
#include <tuple>
#include <string>
#include <memory>

#include "model_header.pb.h"

namespace newtranx {
namespace ai_server {

class ModelWrapper final {
public:
    /**
     * @brief  extract original model from encrypted model file
     *
     * @param encrypted_model_path   file path of encrypted model
     *
     * @return   decrypted model path (sentencepiece and translate) on ramdisk
     */
    std::tuple<std::string, std::string> decrypt_model(const std::string& encrypted_model_path);

    // umount ramdisk
    void umount_fs();

private:
    /**
     * @brief create ramdisk for keeping decrypted model
     *        mount point dir is kept in mount_path_
     *
     * @param size  size of ramdisk
     */
    void mount_fs(std::size_t size);

    /**
     * @brief  read model header from encrypted model
     *         readed model header is kept in model_header_
     *
     * @param model_path  encrypted model file
     */
    void parse_model_header(const std::string& model_path);

    std::string base64_decode(const std::string &encoded);

    /**
     * @brief decrypt with openssl rsa
     *
     * @param encrypted  encrypted data
     * @param key_file   private key file path
     *
     * @return           decrypted plain text
     */
    std::string decrypt_text(const std::string &encrypted, const std::string &key_file);

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
    int sys_cmd(const std::string &cmd, std::string *out = nullptr,
                std::string *err = nullptr, const std::string &input = "");
    int sys_cmd_in_stream(const std::string &cmd, std::string *out = nullptr,
                    std::string *err = nullptr, std::istream* in_stream = nullptr);
    std::string sys_cmd_simple(const std::string &cmd, const std::string &input = "");

private:
    std::string mount_path_;  // 加载解密后模型的目录
    std::unique_ptr<ModelHeader> model_header_;

    // password for private key for decrypting model header
    static constexpr char passwd_[]{"__frame_dummy_init_array_entry"};

    // encrypted private key with password passwd_
    static constexpr uint8_t encrypted_key_[]{
        83, 97, 108, 116, 101, 100, 95, 95, 107, 220, 215, 112, 213, 236, 184, 113,
        151, 16, 236, 202, 58, 191, 163, 35, 220, 0, 122, 144, 89, 99, 9, 15, 207, 72,
        68, 46, 150, 124, 3, 127, 97, 250, 133, 39, 90, 139, 19, 14, 49, 52, 19, 236,
        40, 9, 138, 218, 65, 96, 34, 186, 99, 206, 121, 167, 120, 92, 58, 228, 176,
        176, 207, 78, 87, 242, 157, 80, 19, 118, 89, 170, 22, 70, 185, 127, 6, 13, 233,
        19, 1, 133, 123, 192, 241, 35, 197, 26, 152, 18, 62, 200, 122, 156, 198, 94,
        166, 173, 45, 62, 246, 156, 84, 245, 80, 170, 68, 110, 169, 226, 106, 232, 80,
        37, 110, 79, 153, 226, 185, 120, 240, 161, 2, 208, 56, 164, 108, 8, 181, 229,
        45, 212, 231, 81, 36, 10, 37, 232, 111, 150, 124, 70, 66, 138, 16, 149, 172,
        252, 11, 37, 183, 188, 13, 30, 96, 4, 136, 76, 76, 72, 105, 167, 78, 104, 179,
        113, 157, 146, 229, 136, 138, 75, 11, 20, 182, 23, 62, 3, 16, 36, 148, 109,
        154, 175, 176, 7, 122, 46, 31, 168, 12, 85, 90, 140, 23, 117, 245, 169, 161,
        244, 162, 149, 5, 63, 69, 123, 146, 202, 188, 131, 18, 177, 154, 47, 69, 173,
        85, 169, 37, 232, 208, 173, 213, 110, 217, 39, 106, 201, 110, 83, 176, 94, 32,
        185, 18, 73, 154, 46, 24, 117, 72, 21, 167, 84, 114, 20, 211, 139, 118, 115,
        202, 19, 254, 9, 158, 137, 49, 152, 127, 52, 93, 161, 133, 19, 134, 116, 132,
        155, 135, 165, 17, 2, 154, 117, 21, 196, 26, 17, 46, 216, 46, 170, 98, 231,
        214, 253, 162, 182, 86, 38, 145, 192, 219, 235, 170, 34, 1, 50, 244, 118, 235,
        22, 237, 171, 110, 75, 139, 211, 69, 216, 125, 22, 59, 153, 58, 2, 151, 194,
        232, 168, 161, 108, 247, 181, 170, 122, 134, 142, 136, 72, 155, 199, 12, 243,
        129, 6, 67, 159, 22, 183, 66, 215, 88, 88, 47, 252, 73, 90, 104, 77, 122, 237,
        102, 77, 26, 104, 222, 103, 239, 183, 255, 245, 192, 40, 149, 53, 163, 188,
        130, 211, 103, 29, 142, 253, 190, 148, 196, 52, 102, 67, 202, 120, 22, 212, 90,
        80, 114, 244, 192, 78, 188, 26, 212, 135, 211, 63, 228, 104, 249, 250, 121,
        206, 218, 110, 185, 38, 88, 49, 108, 69, 17, 254, 148, 208, 131, 105, 215, 231,
        160, 244, 13, 200, 244, 188, 231, 198, 153, 9, 162, 225, 72, 185, 224, 25, 166,
        37, 89, 249, 125, 51, 219, 129, 51, 42, 152, 127, 81, 244, 80, 20, 210, 47, 75,
        137, 2, 146, 164, 95, 47, 160, 58, 199, 130, 204, 237, 182, 229, 218, 252, 136,
        203, 168, 150, 129, 166, 2, 57, 234, 127, 17, 109, 128, 31, 220, 176, 238, 96,
        191, 146, 172, 128, 195, 212, 122, 168, 135, 198, 224, 38, 80, 169, 249, 43,
        80, 222, 225, 203, 12, 56, 94, 148, 209, 56, 149, 95, 107, 43, 251, 4, 216,
        250, 227, 105, 228, 183, 163, 192, 116, 239, 230, 79, 63, 160, 186, 44, 157, 2,
        131, 176, 212, 100, 192, 244, 108, 242, 127, 131, 5, 251, 214, 98, 219, 126,
        243, 38, 171, 85, 137, 21, 60, 27, 4, 249, 78, 63, 29, 40, 169, 55, 83, 170,
        86, 80, 46, 37, 252, 177, 136, 164, 166, 162, 96, 19, 90, 119, 62, 245, 205,
        17, 160, 213, 88, 109, 211, 16, 124, 92, 72, 145, 246, 105, 81, 126, 242, 96,
        220, 51, 178, 203, 81, 132, 51, 145, 167, 228, 182, 227, 176, 205, 99, 81, 230,
        247, 171, 161, 67, 199, 143, 44, 85, 231, 240, 67, 15, 128, 206, 178, 193, 203,
        201, 70, 215, 182, 123, 76, 196, 158, 47, 82, 54, 148, 129, 103, 174, 184, 233,
        97, 69, 255, 213, 241, 30, 178, 173, 161, 117, 11, 102, 182, 84, 240, 73, 238,
        184, 214, 122, 201, 241, 172, 161, 123, 202, 19, 58, 175, 254, 250, 153, 129,
        132, 181, 123, 24, 82, 72, 205, 126, 7, 84, 136, 56, 84, 126, 251, 26, 132,
        101, 176, 196, 207, 90, 55, 95, 43, 97, 160, 133, 165, 219, 19, 109, 189, 32,
        225, 43, 21, 78, 236, 31, 167, 192, 154, 81, 148, 10, 252, 47, 102, 92, 194,
        61, 80, 203, 232, 45, 84, 249, 213, 131, 62, 182, 16, 39, 219, 102, 42, 10, 80,
        36, 93, 193, 44, 189, 218, 10, 93, 193, 44, 148, 186, 70, 184, 146, 212, 144,
        71, 174, 72, 129, 27, 95, 32, 13, 18, 24, 135, 131, 125, 212, 119, 40, 75, 220,
        133, 166, 51, 29, 131, 242, 170, 122, 138, 14, 62, 70, 142, 41, 43, 71, 13,
        215, 116, 66, 152, 229, 187, 104, 235, 41, 247, 24, 44, 75, 8, 185, 204, 37,
        130, 204, 102, 125, 9, 96, 77, 162, 68, 99, 225, 40, 244, 50, 50, 254, 203,
        236, 110, 138, 106, 3, 31, 199, 93, 225, 89, 46, 175, 250, 189, 89, 190, 238,
        103, 19, 89, 179, 170, 17, 137, 155, 60, 10, 223, 85, 170, 204, 195, 21, 244,
        21, 50, 72, 84, 171, 128, 180, 16, 1, 17, 16, 102, 124, 131, 83, 198, 196, 136,
        151, 40, 74, 159, 143, 173, 174, 102, 11, 78, 37, 124, 196, 219, 90, 135, 226,
        48, 161, 48, 246, 8, 201, 19, 111, 51, 184, 162, 92, 88, 6, 161, 244, 143, 212,
        73, 166, 196, 31, 56
    };
};

}  // namespace ai_server
}  // namespace newtranx

#if 0 // private key for model header decryption
-----BEGIN PRIVATE KEY-----
MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBAMaxLkrlTlCw2pNo
CpsiRsIhaObnWRVWUraFZcxf+yfUVxjN4KuqGIx+PltfZhz7ewpMOBArTc+Hv760
fFoYIgCuTLyzM5V8yhjBuXB1pxhAj2eL6x2fFf/Jb4+5WqDzMhS1gdj+o7pDnRZg
5dYo7on5QzmNi1kyJmf6QYcSGR8fAgMBAAECgYEAoydk4v1OhLXklfxuERthTRHQ
KSM5pGrA+r2pbNw3yWn0pxqPbYb8J4mWvMMmf5AFZdxWgPh+KXbH/doI7pY4fC+t
S9T6FASuxodQY2iDlhPv7cULzfCnpYlMkGt+F6ME1+tpXDwqX2uIJ/nZjqhk7bcA
zffD8RxWcHzDOC07nBECQQD9t1ifnHde2AflIVA/A5KILmhvlbgUsNp8WTzy5NHL
eyEwU1SnBeYDV+aviEdsx4zxV1PlOjSllOBYalBlAmpHAkEAyHsKDZPs+lhJ8hsg
YCm4Z2s4hgNhcBc0mtJMjYGpwnCV3znx7rFBCktOYp1rGuBcMiDRvK/hOK4k7DV3
DkQ4aQJAECa30ZFYWAVQm1l/jkNf9KNfqprRWR/ZCMl1g9DlOhrghf8ICInrlpLl
8I6wXVBmvIZepN0LtO7BSV+bNpLsyQJAMCiWRqK1N4ID6Vuo0FDVUkqIXnDrdCCu
603/Q7LVE7c12VSzEDKWPCHQ55GHDAK/Qu9tUpUxVgtHJVLGka/DmQJBAMQ3cYnJ
oTlfFpQTLp8Al1MZlKy9R17DNqW/mSMKGKPVOJKztAgkrMdLHibImBC4rJsjW8h1
AnGMUYnjaI4CB9Y=
-----END PRIVATE KEY-----
#endif

#if 0 // public key for model header encryption
-----BEGIN PUBLIC KEY-----
MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDGsS5K5U5QsNqTaAqbIkbCIWjm
51kVVlK2hWXMX/sn1FcYzeCrqhiMfj5bX2Yc+3sKTDgQK03Ph7++tHxaGCIArky8
szOVfMoYwblwdacYQI9ni+sdnxX/yW+PuVqg8zIUtYHY/qO6Q50WYOXWKO6J+UM5
jYtZMiZn+kGHEhkfHwIDAQAB
-----END PUBLIC KEY-----
#endif
