#include <iostream>
#include "boost/fdstream.hpp"

void test_fdostream()
{
    boost::fdostream out(1);    // stream with buffer writing to file descriptor 1

    out << "31 hexadecimal: " << std::hex << 31 << std::endl;
}

void test_fdstreams()
{
    boost::fdistream in(0);     // stream with buffer reading from file descriptor 0
    boost::fdostream out(1);    // stream with buffer writing to file descriptor 1

    out << "read 20 chars and print them (2 twice due to use of unget())"
        << std::endl;

    char c;
    for (int i=1; i<=20; i++) {
        // read next character (out of the buffer)
        in.get(c);

        // print that character (and flush)
        out << c << std::flush;

        // after eight characters, put two characters back into the stream
        if (i == 8) {
            in.unget();
            in.unget();
        }
    }
    out << std::endl;
}


int main()
{
    test_fdostream();
    test_fdstreams();
}

