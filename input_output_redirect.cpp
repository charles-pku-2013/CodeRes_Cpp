#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

void test_output_redirect() {
    ofstream ofs("/tmp/out.txt", ios::out);
    std::streambuf *coutRdbuf = cout.rdbuf();
    cout.rdbuf(ofs.rdbuf());  // for restore
    cout << "Redirect to file" << endl;
    cout.rdbuf(coutRdbuf);  // restore
    cout << "Print to screen" << endl;
}

void test_input_redirect() {
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
}

int main()
{
    test_output_redirect();
    return 0;
}

