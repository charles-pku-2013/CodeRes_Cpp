#include <iostream>
#include <atomic>
#include <type_traits> // 引入类型判断工具
#include <thread>
#include <vector>

// 通用原子加法模板函数
template <typename T>
void atomic_add(std::atomic<T>& atom, T value) {
    // 安全检查：确保只用于数值类型（整型或浮点型）
    static_assert(std::is_arithmetic_v<T>, "atomic_add 只支持数值类型 (整型或浮点型)！");

    if constexpr (std::is_integral_v<T>) {
        // 如果是整型 (int, long, char 等)，直接调用硬件优化的 fetch_add
        atom.fetch_add(value);
    } else {
        // 如果是浮点型 (double, float)，使用 CAS 循环
        T current = atom.load();
        while (!atom.compare_exchange_weak(current, current + value)) {
            // 失败时，current 会自动更新为最新值，继续下一次循环
        }
    }
}

int main() {
    // 测试 1：浮点型 atomic<double>
    std::atomic<double> atomic_double{0.0};
    atomic_add(atomic_double, 1.5); // 匹配 CAS 循环分支
    std::cout << "Double 结果: " << atomic_double.load() << "\n"; // 输出: 1.5

    // 测试 2：整型 atomic<int>
    std::atomic<int> atomic_int{0};
    atomic_add(atomic_int, 42);     // 匹配 fetch_add 分支
    std::cout << "Int 结果: " << atomic_int.load() << "\n";       // 输出: 42

    // 测试 3：不支持的类型（如果你尝试编译下面这行，会触发 static_assert 编译报错）
    // std::atomic<std::string*> atomic_ptr;
    // atomic_add(atomic_ptr, ...); 

    return 0;
}
