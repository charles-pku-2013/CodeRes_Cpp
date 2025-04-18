/*
 *  Effective Modern C++ Item 27
 */
#include <iostream>
#include <string>
#include <set>
#include <type_traits>

using namespace std;

std::set<std::string> names;

template<typename T>
void logAndAddImpl(T&& name, std::false_type) {
    names.emplace(std::forward<T>(name));
    cout << "Not integral " << name << endl;
}

void logAndAddImpl(int idx, std::true_type) {
    cout << "Is intergral " << idx << endl;
}

template<typename T>
void logAndAdd(T&& name) {
    logAndAddImpl(
        std::forward<T>(name), 
        std::is_integral<typename std::remove_reference<T>::type>()
    );
}

int main() {
    logAndAdd("charles");
    logAndAdd(123);

    return 0;
}
