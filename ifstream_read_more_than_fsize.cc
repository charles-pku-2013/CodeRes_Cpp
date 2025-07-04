#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char **argv) {
    std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);
    if (!ifs) {
        cout << "bad file" << endl;
        return -1;
    }

    constexpr int LEN = 1024 * 1024;
    std::vector<char> buf(LEN, 0);

    // read more than file size
    ifs.read(buf.data(), LEN);

    // NOTE 用这种方法判断是否读取成功
    cout << ifs.gcount() << endl;

    // 指定读取长度超过文件长度必定是失败状态
    // if (!ifs) {
        // cout << "fail" << endl;
    // }

    cout << buf.data() << endl;

    return 0;
}
