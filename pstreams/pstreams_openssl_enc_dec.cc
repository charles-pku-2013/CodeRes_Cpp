#include <pstreams/pstream.h>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/format.hpp>

std::string encrypt(const std::string& plain, const std::string& key) {
    std::string cmd = boost::str(boost::format(
        "openssl pkeyutl -encrypt -inkey %s -pubin") % key);

    const redi::pstreams::pmode all3streams =
        redi::pstreams::pstdin | redi::pstreams::pstdout | redi::pstreams::pstderr;

    redi::pstream ps(cmd, all3streams);

    ps << plain << redi::peof;

    std::string result;
    {
        std::stringstream ss;
        ss << ps.out().rdbuf();
        result = ss.str();
    }

    ps.clear();

    std::string err;
    {
        std::stringstream ss;
        ss << ps.err().rdbuf();
        err = ss.str();
    }

    int status = ps.close();
    if (status) {
        std::cerr << "Encrypt fail: " << err << std::endl;
    }

    return result;
}

std::string decrypt(const std::string& encrypted, const std::string& key) {
    std::string cmd = boost::str(boost::format(
        "openssl pkeyutl -decrypt -inkey %s") % key);

    const redi::pstreams::pmode all3streams =
        redi::pstreams::pstdin | redi::pstreams::pstdout | redi::pstreams::pstderr;

    redi::pstream ps(cmd, all3streams);
    ps << encrypted << redi::peof;

    std::string result;
    {
        std::stringstream ss;
        ss << ps.out().rdbuf();
        result = ss.str();
    }

    ps.clear();

    std::string err;
    {
        std::stringstream ss;
        ss << ps.err().rdbuf();
        err = ss.str();
    }

    int status = ps.close();
    if (status) {
        std::cerr << "Encrypt fail: " << err << std::endl;
    }

    return result;
}

int main(int argc, char **argv) {
    using namespace std;

    auto pub_key = argv[1];
    auto pri_key = argv[2];

    std::string plain = "Hello world!";

    std::string encrypted = encrypt(plain, pub_key);
    cout << encrypted.length() << endl;

    std::string decrypted = decrypt(encrypted, pri_key);
    cout << decrypted << endl;

    return 0;
}
