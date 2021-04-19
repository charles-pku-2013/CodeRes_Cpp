#include <iostream>
#include <bitset>
#include <boost/format.hpp>

using namespace std;

int main(int argc, char **argv) {
    int i = 30;
    bitset<64> bs(i);
    cout << bs.to_string() << endl;

    return 0;
}

