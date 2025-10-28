/*
 * From Effective Modern C++ Item 36
 */
#include <iostream>
#include <thread>
#include <future>
#include <type_traits>

#define THIS_THREAD_ID        std::this_thread::get_id()
#define SLEEP_SECONDS(x)      std::this_thread::sleep_for(std::chrono::seconds(x))

// F must be callable
// Ts 可变参数列表
template<typename F, typename... Ts>
inline
// std::future<typename std::result_of_t<F(Ts...)>>    // 返回类型 deprecated
std::future<typename std::invoke_result_t<F, Ts...>>   // c++ 17 standard
realasync(F&& f, Ts&&... params)
{
    return std::async(std::launch::async,
                      std::forward<F>(f),
                      std::forward<Ts>(params)...);
}

int add( int a, int b )
{
    using namespace std;
    cout << "Running add in thread: " << THIS_THREAD_ID << endl;
    SLEEP_SECONDS(3);
    return a + b;
}

int main()
{
    using namespace std;

    cout << "main thread: " << THIS_THREAD_ID << endl;

    auto f1 = realasync(add, 3, 5);
    auto f2 = realasync(add, 4, 9);

    cout << "main thread waiting for results..." << endl;
    cout << f1.get() << endl;
    cout << f2.get() << endl;

    return 0;
}

