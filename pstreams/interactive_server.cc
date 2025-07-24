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
        // ps.open("spm_encode --model 418M/spm.model", all3streams);
        ps.open("/tmp/test", all3streams);

        std::string line;
        while (true) {
            if (ps.fail()) {
                cerr << "Fail to read stream" << endl;
                return;
            }
            getline(ps.out(), line);
            cout << line << endl;
        }
    });

    auto cleanup_fn = [&](void*) {
        cerr << "Waiting thread..." << endl;
        thr.join();
    };
    std::unique_ptr<void, decltype(cleanup_fn)> cleanup((void*)1, cleanup_fn);

    ::sleep(1);

    auto on_fail = [&] {
        ps.clear();
        std::stringstream ss;
        ss << ps.err().rdbuf();
        cerr << ss.str() << endl;
    };

    if (ps.fail()) {
        cerr << "Failed to run cmd" << endl;
        on_fail();
        return -1;
    }

    cerr << "Starting reading..." << endl;
    std::string line;
    while (getline(cin, line)) {
        if (ps.fail()) {
            cerr << "Bad stream" << endl;
            on_fail();
            return -1;
        }
        ps << line << endl;
    }
    ps << redi::peof;

    return 0;
}
