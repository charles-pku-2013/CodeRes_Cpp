/*
 * From Effective Modern C++ Item 36
 */
#include <iostream>
#include <thread>
#include <future>
#include <type_traits>

#define THIS_THREAD_ID        std::this_thread::get_id()

// F must be callable
// Ts 可变参数列表
template<typename F, typename... Ts>
inline
std::future<typename std::result_of<F(Ts...)>::type>    // 返回类型
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
    return a + b;
}

int main()
{
    using namespace std;

    cout << "main thread: " << THIS_THREAD_ID << endl;

    auto f = realasync(add, 3, 5);
    cout << f.get() << endl;

    return 0;
}

