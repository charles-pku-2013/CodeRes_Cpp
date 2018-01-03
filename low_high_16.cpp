#include <iostream>
#include <map>
#include <string>


int main(int argc, char **argv)
{
    using namespace std;

    if (argc <= 1) {
        cerr << "Usage: " << argv[0] << " number" << endl;
        return -1;
    } // if

    int nread = 0;
    uint32_t n = 0;

    if (strncmp(argv[1], "0x", 2) == 0) {
        nread = sscanf(argv[1], "%x", &n);
    } else {
        nread = sscanf(argv[1], "%u", &n);
    } // if

    if (nread != 1) {
        cerr << "Invalid number!" << endl;
        return -1;
    } // if

    uint32_t low = n & 0x0000ffff;
    uint32_t high = (n >> 16) & 0x0000ffff;

    cout << "Input number is: " << n << endl;
    cout << "low 16 bits is: " << low << endl;
    cout << "high 16 bits is: " << high << endl;

    return 0;

}

