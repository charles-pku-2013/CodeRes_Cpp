#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace std;

static
void do_combine(char **argv)
{
    uint64_t val = 0, high = 0, low = 0;
    high = boost::lexical_cast<uint64_t>(argv[1]);
    low = boost::lexical_cast<uint64_t>(argv[2]);

    val = (high << 32) + low;

    cout << val << endl;
}

static
void do_split(char **argv)
{
    uint32_t val = boost::lexical_cast<uint32_t>(argv[1]);
    uint16_t low = (uint16_t)val;
    uint16_t high = (uint16_t)(val >> 16);
    cout << "high=" << high << " low=" << low << endl;
}

int main(int argc, char **argv)
try {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " high low" << endl;
        cerr << "or " << argv[0] << " docid" << endl;
        exit(-1);
    }

    if (argc >= 3) {
        do_combine(argv);
    } else {
        do_split(argv);
    }

    return 0;

} catch (const std::exception &ex) {
    std::cerr << "Exception caught by main: " << ex.what() << std::endl;
    return -1;
} catch (...) {
    std::cerr << "Unknown exception caught by main!" << std::endl;
    return -1;
}



