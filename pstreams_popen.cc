// apt install -y libpstreams-dev
// see test_pstreams.cc

#include <pstreams/pstream.h>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
    const redi::pstreams::pmode all3streams =
        redi::pstreams::pstdin|redi::pstreams::pstdout | redi::pstreams::pstderr;
    redi::ipstream in(argv[1], all3streams);

    std::string str;
    while (std::getline(in.out(), str)) {
        std::cout << "OUT: " << str << std::endl;
    }
    in.clear();
    while (std::getline(in.err(), str)) {
        std::cout << "ERR: " << str << std::endl;
    }

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
