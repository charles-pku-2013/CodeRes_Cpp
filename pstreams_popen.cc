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

    {
        std::stringstream ss;
        ss << ps.out().rdbuf();
        std::cout << "OUT: " << ss.str() << std::endl;
    }

    ps.clear();

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

int sys_cmd(const std::string &cmd, std::string *out, std::string *err,
            const std::string &input) {
    std::istream* in_stream = nullptr;

    std::stringstream ss;
    if (!input.empty()) {
        ss << input << std::flush;
        in_stream = &ss;
    }

    return sys_cmd_in_stream(cmd, out, err, in_stream);
}

int sys_cmd_in_stream(const std::string &cmd, std::string *out, std::string *err,
            std::istream* in_stream) {
    const redi::pstreams::pmode all3streams =
        redi::pstreams::pstdin | redi::pstreams::pstdout | redi::pstreams::pstderr;

    redi::pstream ps(cmd, all3streams);

    if (in_stream) {
        ps << in_stream->rdbuf() << redi::peof;
    }

    if (out) {
        std::stringstream ss;
        ss << ps.out().rdbuf();
        *out = ss.str();
        boost::trim(*out);
    }

    ps.clear();

    if (err) {
        std::stringstream ss;
        ss << ps.err().rdbuf();
        *err = ss.str();
        boost::trim(*err);
    }

    return ps.close();
}

std::string sys_cmd_simple(const std::string &cmd, const std::string &input) {
    std::string out, err;

    int retval = sys_cmd(cmd, &out, &err, input);

    if (retval) {
        throw std::runtime_error(boost::str(boost::format(
            "Failed to run command '%s', error: '%s'"
        ) % cmd % err));
    }

    return out;
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
