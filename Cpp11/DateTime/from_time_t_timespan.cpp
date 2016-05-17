#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <ctime>

#define THIS_THREAD_ID        std::this_thread::get_id()
#define SLEEP_SECONDS(x)      std::this_thread::sleep_for(std::chrono::seconds(x))
#define SLEEP_MILLISECONDS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))

using namespace std;

int main()
{
    using Clock = std::chrono::system_clock;

    time_t time1 = time(0);
    Clock::time_point tp1 = Clock::from_time_t(time1);

    SLEEP_SECONDS(3);

    time_t time2 = time(0);
    Clock::time_point tp2 = Clock::from_time_t(time2);

    // elapsed is std::chrono::duration 
    auto elapsed = tp2 - tp1;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed);
    cout << seconds.count() << endl;

    return 0;
}
