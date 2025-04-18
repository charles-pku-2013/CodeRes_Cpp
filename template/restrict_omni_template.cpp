/*
 *  Effective Modern C++ Item 27
 */
#include <iostream>
#include <string>
#include <set>
#include <type_traits>
#include <utility>

using namespace std;

// NOTE std::decay<T> remove reference and cv (const, volatile)
class Person {
 public:
    // The 2nd anonymous template arg is a type
    // std::enable_if<VALUE true or false>
    // std::decay<T>::type is a type
    // std::is_same expect 2 types
    template<
             typename T,
             typename = typename std::enable_if<
                             !std::is_same<Person, typename std::decay<T>::type>::value
                         >::type
            >
    explicit Person(T&& n)
            : name_(std::forward<T>(n))
    {
        cout << "Arg is not Person " << n << endl;
    }

    std::string name_;
};

// std::is_base_of<Base, Derived>::value == True
class Animal {
 public:
    // template<typename T>
    // 如果不加限制，万能模板将压制复制构造函数
    // template<typename T,
             // typename = typename std::enable_if<
                    // !std::is_base_of<Animal, typename std::decay<T>::type>::value
                // >::type
            // >
    // 和上面等价但更简洁
    // template<typename T,
             // typename = std::enable_if_t<
                                            // !std::is_base_of<Animal, typename std::decay<T>::type>::value
                                        // >
            // >
    // multi restriction condition
    template<typename T,
             typename = std::enable_if_t<
                                            !std::is_base_of<Animal, typename std::decay<T>::type>::value
                                            &&
                                            !std::is_integral<std::remove_reference_t<T>>::value
                                        >
            >
    explicit Animal(T&& n)
            : name_(std::forward<T>(n)) {
        static_assert(
            std::is_constructible<std::string, T>::value,
            "Parameter n cannot be used to build a std::string!"
        );
        cout << "Restricted template construct" << endl;
    }  // std::is_constructible<Dst, Src>

    // template<typename T>
    // explicit Animal(T&& n)
        // : name_(std::forward<T>(n))
    // {
        // cout << "Omni template construct" << endl;
    // }

    Animal(const Animal& rhs)
        : name_(rhs.name_)
    {
        cout << "Animal copy construct" << endl;
    }

    std::string name_;
};

int main() {
    // Person p1("charles");

    Animal d1("mary");
    Animal d2(d1);
    Animal d3(123);

    return 0;
}
