#include <ctime>
#include <iostream>

using namespace std;

int main() {
    std::time_t now = std::time(0);
    std::tm *ptm = std::localtime(&now);
    char buffer[32];
    std::strftime(buffer, 32, "%Y%m%d%H%M%S", ptm); 

    cout << buffer << endl;

    return 0;
}

