#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>


int run_cmd(const std::string &cmd, std::string &output)
{
    using namespace std;

    int retval = 0;

    string cmdStr = "stdbuf -oL ";
    cmdStr.append(cmd).append(" 2>&1");

    FILE *fp = popen(cmdStr.c_str(), "r");
    setlinebuf(fp);

    typedef boost::iostreams::stream< boost::iostreams::file_descriptor_source >
                    FDRdStream;
    FDRdStream ppStream(fileno(fp), boost::iostreams::never_close_handle);

    stringstream ss;
    ss << ppStream.rdbuf();

    output = ss.str();

    retval = pclose(fp);
    retval = WEXITSTATUS(retval);

    return retval;
}


int main(int argc, char **argv)
{
    using namespace std;

    string output;
    int retcode = run_cmd("/tmp/test.sh", output);

    cout << output << endl;
    cout << "retcode = " << retcode << endl;

    return 0;
}
