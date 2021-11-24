#include <vector>
#include <iostream>

int main() {
    using namespace std;

    const int cnt = 10;
    std::vector<bool> status(cnt, false);
    auto& state = status[0];  // NOTE!!! error: non-const lvalue reference to type '__bit_reference<...>' cannot bind to a temporary of type '__bit_reference<...>'
    // NOTE!!! vector<bool> reference_type 就是 bool 值类型
    // 同样也不能用于lambda表达式capture ref
    cout << state << endl;

    return 0;
}

