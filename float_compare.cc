#include <iostream>
#include <cmath> // 必须包含 cmath 以使用 std::abs
#include <limits>    // 包含 numeric_limits
#include <algorithm> // 包含 std::max

// 高精度要求
bool isRelativelyEqual(double a, double b) {
    double diff = std::abs(a - b);
    double max_val = std::max(std::abs(a), std::abs(b));

    // epsilon() 返回该类型能表示的 1 与大于 1 的最小浮点数之差
    // 乘以两者中的最大值，动态调整误差范围
    return diff <= max_val * std::numeric_limits<double>::epsilon();
}

// 在 C 语言中，你必须用 fabs；但在 C++ 中，推荐使用 std::abs，因为 C++ 支持函数重载。
bool isAlmostEqual(double a, double b, double epsilon = 1e-5) {
    return std::abs(a - b) <= epsilon;
}

int main() {
    double a = 0.1 + 0.2;
    double b = 0.3;

    if (a == b) {
        std::cout << "直接使用 == : 相等\n"; // 这行通常不会执行
    } else {
        std::cout << "直接使用 == : 不相等\n"; // 会执行这行
    }

    if (isAlmostEqual(a, b)) {
        std::cout << "使用 Epsilon: 相等\n"; // 会执行这行
    }

    if (isRelativelyEqual(a, b)) {
        std::cout << "使用 Numeric Limit Epsilon: 相等\n"; // 会执行这行
    }

    return 0;
}
