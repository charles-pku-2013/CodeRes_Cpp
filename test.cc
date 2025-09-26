#include <iostream>
#include <chrono>
#include <fmt/chrono.h>

int main() {
    // Get the current time point
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);

    // Print the local time using fmt::localtime and the "%c" specifier
    // "%c" produces the locale's date and time representation
    fmt::print("Local date and time: {:%c}\n", fmt::localtime(timestamp));

    // Or use custom format specifiers for more control
    fmt::print("Local time (HH:MM:SS): {:%H:%M:%S}\n", fmt::localtime(timestamp));

    // The `%Z` specifier prints the time zone abbreviation
    fmt::print("Local time with time zone: {:%H:%M:%S %Z}\n", fmt::localtime(timestamp));
    fmt::println("The date is {:%Y-%m-%d}.", *std::localtime(&t));

    return 0;
}
