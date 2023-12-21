#include <iostream>
#include <iomanip>
#include <cmath>

int main(int argc, char **argv) {
    using namespace std;
    int i = atoi(argv[1]);

    switch(i) {
        case 1: {
            cout << "100" << endl;
        }
        case 2: {
            cout << "200" << endl;
            break;
        }
        default:
            break;
    }

    return 0;
}

/*
 * 0.331034481525421142578125
 * 0.331034480000000019828121367027051746845245361328125
 */
