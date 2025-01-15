#include <iostream>
#include <fstream>
#include <memory>
#include <boost/crc.hpp>
#include <boost/format.hpp>

using namespace std;

int main(int argc, char **argv) {
    std::ifstream in_file(argv[1], std::ios::in | std::ios::binary);
    in_file.seekg(0, ios::end);
    std::size_t file_size = in_file.tellg();
    cout << file_size << endl;
    in_file.seekg(0);

    std::shared_ptr<char> pData(new char[file_size], [](char *data){
        cout << "free" << endl;
        delete [] data;
    });

    in_file.read(pData.get(), file_size);

    std::string s(pData.get(), pData.get() + file_size);
    cout << s << endl;
    cout << s.length() << endl;

    std::shared_ptr<void> pVoid = std::static_pointer_cast<void>(pData);
    cout << pVoid.use_count() << endl;
    pData.reset();
    cout << pVoid.use_count() << endl;

    boost::crc_32_type result;
    result.process_bytes(pVoid.get(), file_size);
    uint32_t cksum = result.checksum();
    cout << boost::format("%lx") % cksum << endl;

    cin.get();

    return 0;
}

