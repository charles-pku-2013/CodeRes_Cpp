#include <iostream>
#include <vector>
#include <sstream>
#include <type_traits>

using namespace std;

// Primary template with a static assertion
// for a meaningful error message
// if it ever gets instantiated.
// We could leave it undefined if we didn't care.
template <typename, typename T> struct has_serialize {
    static_assert(std::integral_constant<T, false>::value,
            "Second template parameter needs to be of function type.");
};

// specialization that does the checking
template <typename C, typename Ret, typename... Args>
struct has_serialize<C, Ret(Args...)> {
private:
    template <typename T>
        static constexpr auto check(T *) -> typename std::is_same<
        decltype(std::declval<T>().serialize(std::declval<Args>()...)),
        Ret      // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        >::type; // attempt to call it and see if the return type is correct

    template <typename> static constexpr std::false_type check(...);

    typedef decltype(check<C>(0)) type;

public:
    static constexpr bool value = type::value;
};

template <typename, typename T> struct has_size {
    static_assert(std::integral_constant<T, false>::value,
            "Second template parameter needs to be of function type.");
};

// specialization that does the checking
template <typename C, typename Ret, typename... Args>
struct has_size<C, Ret(Args...)> {
private:
    template <typename T>
        static constexpr auto check(T *) -> typename std::is_same<
        decltype(std::declval<T>().size(std::declval<Args>()...)),
        Ret      // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        >::type; // attempt to call it and see if the return type is correct

    template <typename> static constexpr std::false_type check(...);

    typedef decltype(check<C>(0)) type;

public:
    static constexpr bool value = type::value;
};

// usage
struct X {
    int serialize(const std::string &) { return 42; }
};

struct Y : X {
    std::string serialize() const; // const doesn't make diff
};

int main() {
    // std::cout << has_serialize<Y, int(const std::string &)>::value << std::endl; // will print 1
    // std::cout << has_serialize<Y, std::string(void)>::value << std::endl;

    std::vector<int> arr{1,2,3};
    cout << has_size<decltype(arr), std::size_t(void)>::value << endl;
    cout << has_size<std::stringstream, std::size_t(void)>::value << endl;
    cout << has_size<Y, std::size_t(void)>::value << endl;

    if (has_size<decltype(arr), std::size_t(void)>::value) {
        cout << arr.size() << endl;
    } else {
        cout << "No size!" << endl;
    }

    return 0;
}
