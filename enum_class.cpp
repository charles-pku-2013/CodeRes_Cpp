#include <iostream>

using namespace std;

enum class Status : std::int32_t {
    OK = 0,
    FAILED = -1,
    CANCELED = 1,
    INTERRUPTED = 200,
    UNKNOWN = 300
};

void test(int32_t stat) {
    cout << stat << endl;
}

int main() {
    // test(Status::UNKNOWN);  // WRONG
    test(static_cast<int32_t>(Status::UNKNOWN)); // 如果用了 enum class, 必须用 full qualifier

    return 0;
}
