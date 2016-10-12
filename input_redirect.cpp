#include <iostream>
#include <sstream>

using namespace std;


int main()
{
    stringstream sstream("1 3 5 7");

    // for restore
    std::streambuf *cinRdbuf = cin.rdbuf();
    cin.rdbuf(sstream.rdbuf());

    int n;
    while (cin >> n)
        cout << n << endl;

    // sstream.clear();
    // sstream.seekg(0); sstream.seekp(0);
    sstream.str("20 30 50");
    cin.clear();
    while (cin >> n)
        cout << n << endl;

    // not read to eof
    sstream.str("70 80 90");
    cin.clear();
    cin >> n;
    cout << n << endl;

    sstream.str("100 200");
    // cin.clear();
    cin >> n;
    cout << n << endl;

    cin.rdbuf(cinRdbuf);
    // cin >> n;

    return 0;
}

