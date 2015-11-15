#include <cstdio>
#include <cstring>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>


using namespace std;


int main()
{
    typedef std::chrono::system_clock Clock;

    auto now = Clock::now();
    auto d = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(d);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(d);
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(d);
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(d);

    cout << seconds.count() << endl;
    cout << milliseconds.count() << endl;
    cout << microseconds.count() << endl;
    cout << nanoseconds.count() << endl;

    return 0;
}
