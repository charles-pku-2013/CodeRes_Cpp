#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <variant>
#include <vector>

using namespace std;

template <typename, typename T>
struct has_size
{
    static_assert(std::integral_constant<T, false>::value, "Second template parameter needs to be of function type.");
};

template <typename C, typename Ret, typename... Args>
struct has_size<C, Ret(Args...)>
{
private:
    template <typename T>
    static constexpr auto check(T*) ->
        typename std::is_same<decltype(std::declval<T>().size(std::declval<Args>()...)),
                              Ret      // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                              >::type; // attempt to call it and see if the return type is correct

    template <typename>
    static constexpr std::false_type check(...);

    typedef decltype(check<C>(0)) type;

public:
    static constexpr bool value = type::value;
};

template <typename T>
static typename std::enable_if<has_size<T, std::size_t(void)>::value>::type get_size(const T& val)
{
    cout << val.size() << endl;
}

template <typename T>
static typename std::enable_if<!has_size<T, std::size_t(void)>::value>::type get_size(const T&)
{
    cout << "null" << endl;
}

// the variant to visit
using value_t = std::variant<int, long, double, std::string>;

int main() {
    std::vector<value_t> vec = {10, 15l, 1.5, "hello"};

    for (auto& v : vec) {
        std::visit(
            [](auto&& arg) {
                cout << typeid(arg).name() << endl;
            },
            v
        );
        std::visit(
            [](auto&& arg){ get_size(std::forward<decltype(arg)>(arg)); },
            v
        );
    }
    cout << endl;

    // can operate multi values
    std::visit(
        [](auto&& arg1, auto&& arg2) {
            // NOTE, static_assert cannot be used here, mybe here is runtime
            // static_assert(std::is_same_v<
                // std::remove_reference<decltype(arg1)>,
                // std::remove_reference<decltype(arg2)>> == true);
            cout << std::is_same_v<
                std::remove_reference<decltype(arg1)>,
                std::remove_reference<decltype(arg2)>> << endl;
            // cout << typeid(arg1).name() << " " << typeid(arg2).name() << endl;
            cout << arg1 << " " << arg2 << endl;
        },
        vec[0], vec[3]
    );

    return 0;
}
