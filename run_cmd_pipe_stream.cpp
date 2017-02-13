/*
 * c++ -o /tmp/test run_cmd_pipe_stream.cpp -lboost_iostreams -std=c++11 -pthread -g
 */
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <memory>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>


// static std::string *pstrOutput = NULL;
// static pid_t        g_chldPid;
// static int          g_nExtCode = 0;

static 
void sig_pipe(int signo)
{
    // *pstrOutput = "pipe read/write error!";
    // siglongjmp
    std::cerr << "pipe read/write error!" << std::endl;
}

// static 
// void sig_chld(int signo)
// {
    // pid_t   pid;
    // int     stat;

    // while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {

    // } // while
// }


int run_cmd(const std::string &cmd, const std::string &input, std::string &output)
{
    using namespace std;

    int     retval = 0, stat = 0;
    int     fd1[2], fd2[2];
    pid_t   pid;
    std::string cmdStr;

    cmdStr = "stdbuf -oL ";
    cmdStr.append(cmd).append(" 2>&1");

    if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
        exit(-1);

    if (pipe(fd1) < 0 || pipe(fd2) < 0)
        exit(-1);

    if ((pid = fork()) < 0) {
        output = "Spawn child process error!";
        return -1;

    } else if (0 == pid) {      /* child */
        close(fd1[1]);
        close(fd2[0]);
        
        if (fd1[0] != STDIN_FILENO) {
            if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO)
                exit(-1);
            close(fd1[0]);
        } // if

        if (fd2[1] != STDOUT_FILENO) {
            if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO)
                exit(-1);
            close(fd2[1]);
        } // if

        if (execl("/bin/sh", "sh", "-c", cmdStr.c_str(), (char *)0) < 0) {
            output = "Child process run cmd error!";
            exit(-1);
        } // if
    } // if pid

    /* parent */
	close(fd1[0]);
	close(fd2[1]);

    typedef boost::iostreams::stream< boost::iostreams::file_descriptor_source >
                    FDRdStream;
    typedef boost::iostreams::stream< boost::iostreams::file_descriptor_sink >
                    FDWrStream;
    FDRdStream pipeRdStream( fd2[0], boost::iostreams::close_handle );
    FDWrStream pipeWrStream( fd1[1], boost::iostreams::close_handle );

    // send input
    if (!input.empty()) {
        stringstream ss(input);
        string line;
        while (getline(ss, line))
            pipeWrStream << line << "\n" << flush;
    } // if

    // read output
    string line;
    stringstream ss;
    while (getline(pipeRdStream, line)) {
        ss << line << "\n";
        cout << line << endl;
    } // while
    output = ss.str();

    if (waitpid(pid, &stat, 0) != pid) {
        output = "waitpid error!";
        return -1;
    } // if

    retval = WEXITSTATUS(stat);

    return retval;
}


int main()
{
    using namespace std;

    string output;
    // int ret = run_cmd("ls -l ~/abc", "", output);
    int ret = run_cmd("/tmp/test1", "3\n5", output);

    cout << "\noutput is:\n " << output << endl;
    cout << "retcode = " << ret << endl;

    return 0;
}




#if 0
#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    int n = 0;

    cout << "Please input n for 1st round test: " << endl;
    cin >> n;
    for (int i = 1; i <= n; ++i) {
        sleep(1);
        cout << "1st round test: " << i << endl;
    } // for

    cout << "Please input n for 2nd round test: " << endl;
    cin >> n;
    for (int i = 1; i <= n; ++i) {
        sleep(1);
        cout << "2nd round test: " << i << endl;
    } // for

    return 0;
}
#endif
