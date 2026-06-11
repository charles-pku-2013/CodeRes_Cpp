#include <functional>
#include <type_traits>
#include <utility>
#include <iostream>

template <typename F, typename... Args>
decltype(auto) call(F&& func, Args&&... args)
    noexcept(std::is_nothrow_invocable_v<F, Args...>)
{
    return std::invoke(
        std::forward<F>(func),
        std::forward<Args>(args)...
    );
}

inline constexpr auto lambda_call = [](auto&& func, auto&&... args)
    noexcept(std::is_nothrow_invocable_v<decltype(func), decltype(args)...>)
    -> decltype(auto)
{
    return std::invoke(
        std::forward<decltype(func)>(func),
        std::forward<decltype(args)>(args)...
    );
};

int add(int a, int b)
{
    return a + b;
}

struct Person {
    std::string name;
    int age = 0;
    void say(const std::string& prefix) {
        std::cout << prefix << ", " << name << '\n';
    }
    int get_age() const {
        return age;
    }
    int& age_ref() {
        return age;
    }
    static int static_add(int a, int b) {
        return a + b;
    }
};

int main()
{
    // 普通函数
    std::cout << call(add, 1, 2) << '\n';
    // lambda
    auto mul = [](int a, int b) {
        return a * b;
    };
    std::cout << call(mul, 3, 4) << '\n';
    // 成员函数，对象
    Person p{"Alice", 18};
    call(&Person::say, p, "hello");
    // 成员函数，指针
    call(&Person::say, &p, "hi");
    // const 成员函数
    const Person cp{"Bob", 20};
    std::cout << call(&Person::get_age, cp) << '\n';
    // 静态成员函数
    std::cout << call(&Person::static_add, 10, 20) << '\n';
    // 成员变量
    std::cout << call(&Person::name, p) << '\n';
    // 修改成员变量
    call(&Person::age, p) = 30;
    std::cout << p.age << '\n';
    // 返回引用
    call(&Person::age_ref, p) = 40;
    std::cout << p.age << '\n';
    // 智能指针调用成员函数
    auto sp = std::make_shared<Person>();
    sp->name = "Charlie";
    sp->age = 25;
    call(&Person::say, sp, "hey");

    std::cout << "Lambda test" << std::endl;
    std::cout << lambda_call(add, 1, 2) << std::endl;
    call(&Person::say, p, "hello");

    return 0;
}
