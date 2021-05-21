/*
c++ -o /tmp/test run_cmd.cpp -lboost_iostreams -std=c++11 -g
 */
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <memory>
#include <sys/types.h>
#include <sys/wait.h>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>

/*
 * NOTE! popen 只是读取stdout，所以最好在cmd加上 2>&1
 */
int run_cmd(const std::string &cmd, std::string *out)
{
#if 0
    std::unique_ptr<FILE, std::function<void(FILE*)>> pp(
        ::popen(cmd.c_str(), "r"),
        [&retval](FILE* p) {
            if(p) {
                retval = ::pclose(p);
                retval = WEXITSTATUS(retval);
            }
        }
    );
    // remember manually call pp.reset() before return
#endif
    FILE *fp = ::popen(cmd.c_str(), "r");
    if (!fp) {
        *out = "popen error!";
        return -1;
    }
    int retval = -1;
    std::unique_ptr<void, std::function<void(void*)>> _on_finish((void*)1, [&fp, &retval](void*){
        if (fp) {
            retval = ::pclose(fp);
            retval = WEXITSTATUS(retval);
        }
    });
    ::setlinebuf(fp);
    // ::setvbuf(fp, NULL, _IOLBF, 0);

    using PipeStream = boost::iostreams::stream<boost::iostreams::file_descriptor_source>;
    PipeStream stream(fileno(fp), boost::iostreams::never_close_handle);

    std::stringstream ss;
    ss << stream.rdbuf();

    *out = ss.str();

    _on_finish.reset();
    return retval;
}

// Use this one
int run_cmd2(const std::string &cmd, std::string *out) {
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
        *out = "popen error!";
        return -1;
    }
    ::setlinebuf(pp.get());
    // ::setvbuf(pp.get(), NULL, _IOLBF, 0);

    using PipeStream = boost::iostreams::stream<boost::iostreams::file_descriptor_source>;
    PipeStream stream(fileno(pp.get()), boost::iostreams::never_close_handle);

    std::stringstream ss;
    ss << stream.rdbuf();

    *out = ss.str();

    pp.reset();   // must call it manually
    return retval;
}

int main(int argc, char **argv) {
    using namespace std;

    std::string out;
    int retval = run_cmd2(argv[1], &out);
    cout << "out: " << out << endl;
    cout << "retval: " << retval << endl;

    return 0;
}

