
// apt install -y libpstreams-dev
// see test_pstreams.cc

#include <pstreams/pstream.h>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <unistd.h>

using namespace std;

int main(int argc, char **argv) {
    const redi::pstreams::pmode all3streams =
        redi::pstreams::pstdin | redi::pstreams::pstdout | redi::pstreams::pstderr;

    redi::pstream ps;

    std::thread thr([&]{
        // NOTE 注意工作目录，如果找不到输入文件不会报错
        ps.open("spm_encode --model 418M/spm.model", all3streams);

        std::string line;
        while (getline(ps.out(), line)) {
            cout << line << endl;
        }
    });

    ::sleep(1);

    if (ps.fail()) {
        cerr << "Failed to run cmd" << endl;
        thr.join();
        return -1;
    }

    cerr << "Starting reading..." << endl;
    std::string line;
    while (getline(cin, line)) {
        ps << line << endl;
    }
    ps << redi::peof;

    thr.join();

    return 0;
}
