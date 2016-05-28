#include <iostream>
#include <iomanip>
#include <locale>
#include <sstream>
 
int main()
{
    using namespace std;

    stringstream stream;

    string ip = "123.123.123.123";
    uint16_t port = 10081;
    // cout.imbue(locale("en_US.utf8"));
    stream << "Left fill:\n" << left
              << setw(30) << ip << setw(0) << ":" << port;
    stream << setw(12) << -1.23
              << setw(12) << hex << showbase << 42
              << setw(12) << put_money(123, true) << endl;

    stream.flush();
    cout << stream.str() << endl;
 
    return 0;
}

/*
int main()
{
    std::cout.imbue(std::locale("en_US.utf8"));
    std::cout << "Left fill:\n" << std::left << std::setfill('*')
              << std::setw(12) << -1.23  << '\n'
              << std::setw(12) << std::hex << std::showbase << 42 << '\n'
              << std::setw(12) << std::put_money(123, true) << "\n\n";
 
    std::cout << "Internal fill:\n" << std::internal
              << std::setw(12) << -1.23  << '\n'
              << std::setw(12) << 42 << '\n'
              << std::setw(12) << std::put_money(123, true) << "\n\n";
 
    std::cout << "Right fill:\n" << std::right
              << std::setw(12) << -1.23  << '\n'
              << std::setw(12) << 42 << '\n'
              << std::setw(12) << std::put_money(123, true) << '\n';
}
*/
