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
#include <typeinfo>


using namespace std;


int main()
{
    char buf[20];

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
    snprintf(buf, 17, "%016ld", microseconds.count());
    cout << buf << endl;

    cout << typeid(now).name() << endl;  // NSt3__16chrono10time_pointINS0_12system_clockENS0_8durationIxNS_5ratioILl1ELl1000000EEEEEEE
    cout << typeid(d).name() << endl;  // NSt3__16chrono8durationIxNS_5ratioILl1ELl1000000EEEEE
    cout << typeid(seconds).name() << endl;  // NSt3__16chrono8durationIxNS_5ratioILl1ELl1EEEEE

    return 0;
}
