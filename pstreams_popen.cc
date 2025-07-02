// apt install -y libpstreams-dev
// see test_pstreams.cc

#include <pstreams/pstream.h>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char **argv) {
    const redi::pstreams::pmode all3streams =
        redi::pstreams::pstdin | redi::pstreams::pstdout | redi::pstreams::pstderr;

    redi::pstream ps(argv[1], all3streams);

    std::cout << "IS_EXITED: " << ps.rdbuf()->exited() << std::endl;

    // std::string str;
    // while (std::getline(ps.out(), str)) {
        // std::cout << "OUT: " << str << std::endl;
    // }
    {
        std::stringstream ss;
        ss << ps.out().rdbuf();
        std::cout << "OUT: " << ss.str() << std::endl;
    }
    ps.clear();
    // while (std::getline(ps.err(), str)) {
        // std::cout << "ERR: " << str << std::endl;
    // }
    {
        std::stringstream ss;
        ss << ps.err().rdbuf();
        std::cout << "ERR: " << ss.str() << std::endl;
    }

    std::cout << "IS_EXITED: " << ps.rdbuf()->exited() << std::endl;
    std::cout << "ERROR: " << ps.rdbuf()->error() << std::endl;   // errno
    std::cout << "STATUS: " << ps.rdbuf()->status() << std::endl;
    std::cout << "STATUS: " << ps.close() << std::endl;

    return 0;
}

#if 0
{
    // test reading from bidirectional pstream

    const std::string cmd = "grep '^127' /etc/hosts /no/such/file /dev/stdin";

    pstream ps(cmd, all3streams);

    print_result(ps, ps.is_open());
    check_pass(ps.out());
    check_pass(ps.err());

    ps << "127721\n" << peof;

    std::string buf;
    while (getline(ps.out(), buf))
        cout << "STDOUT: " << buf << endl;
    check_fail(ps);
    ps.clear();
    while (getline(ps.err(), buf))
        cout << "STDERR: " << buf << endl;
    check_fail(ps);
    ps.clear();
}
#endif
