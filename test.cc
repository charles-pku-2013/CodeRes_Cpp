#include <thread>
#include <string>
#include <iostream>

using namespace std;

int main() {
    cout << std::hash<std::thread::id>{}(std::this_thread::get_id()) << endl;
    cout << std::this_thread::get_id() << endl;
    cout << std::to_string(std::this_thread::get_id()) << endl;

    return 0;
}


