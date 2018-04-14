#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace std;

int main(int argc, char **argv)
try {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " high low" << endl;
        exit(-1);
    }

    uint64_t val = 0, high = 0, low = 0;
    high = boost::lexical_cast<uint64_t>(argv[1]);
    low = boost::lexical_cast<uint64_t>(argv[2]);

    val = (high << 32) + low;

    cout << val << endl;

    return 0;

} catch (const std::exception &ex) {
    std::cerr << "Exception caught by main: " << ex.what() << std::endl;
    return -1;
} catch (...) {
    std::cerr << "Unknown exception caught by main!" << std::endl;
    return -1;
}



