/*
 * http://stackoverflow.com/questions/16868129/how-to-store-variadic-template-arguments
 * http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
 * http://stackoverflow.com/questions/10766112/c11-i-can-go-from-multiple-args-to-tuple-but-can-i-go-from-tuple-to-multiple
 */
#include <tuple>
#include <iostream>
#include <functional>
#include <glog/logging.h>

namespace helper
{
    template <std::size_t... Ts>
    struct index {};
    
    template <std::size_t N, std::size_t... Ts>
    struct gen_seq : gen_seq<N - 1, N - 1, Ts...> {
        gen_seq() { DLOG(INFO) << "gen_seq1 construct, N = " << N << " sizeof(Ts) = " << sizeof...(Ts); }
    };
    
    // 类模板特化
    template <std::size_t... Ts>
    struct gen_seq<0, Ts...> : index<Ts...> {
        gen_seq() { DLOG(INFO) << "gen_seq2 construct sizeof(Ts) = " << sizeof...(Ts); }
    };
}


template <typename... Ts>
class Action
{
private:
    std::function<void(Ts...)>      f;
    std::tuple<Ts...>               args;

public:
    Action(std::function<void(Ts...)> func, Ts&&... args) 
            : f(func), args(std::make_tuple(std::forward<Ts>(args)...))
    {}
    
    template <typename... Args, std::size_t... Is>
    void func(std::tuple<Args...>& tup, helper::index<Is...>)
    {
        DLOG(INFO) << "func2 sizeof Is = " << sizeof...(Is);
        f(std::get<Is>(tup)...);
    }
    
    template <typename... Args>
    void func(std::tuple<Args...>& tup)
    {
        DLOG(INFO) << "func1 sizeof Args = " << sizeof...(Args);
        func(tup, helper::gen_seq<sizeof...(Args)>{});
    }
    
    void act()
    {
        func(args);
    }
};

void test(int i, char c, const char *str, double d)
{

}


int main()
{
    using namespace std;

    Action<int,int> add([](int x, int y){std::cout << (x+y) << std::endl;}, 3, 4);
    add.act();

    Action<const char*> printstr([](const char *str){ cout << str << endl; }, "Hello");
    printstr.act();

    const char *str = "Hello";
    Action<int, char, const char*, double> act1(test, 1, 'a', "Hello", 3.14); // 不可以传str
    act1.act();

    return 0;
}
