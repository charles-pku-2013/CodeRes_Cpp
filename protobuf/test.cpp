#include <iostream>
#include "bs.pb.h"

int main(int argc, char **argv)
try {
    using namespace std;

    IndexQueryReq req;

    if (req.ParseFromIstream(&cin)) {
        cout << req.Utf8DebugString() << flush;
    } else {
        cerr << "Parse fail!" << endl;
    }

    return 0;

} catch (const std::exception &ex) {
    std::cerr << "Exception caught by main: " << ex.what() << std::endl;
    return -1;
} catch (...) {
    std::cerr << "Unknown exception caught by main!" << std::endl;
    return -1;
}



