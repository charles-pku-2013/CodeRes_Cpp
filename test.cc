#include <iostream>
#include <type_traits>
#include <vector>

using namespace std;

template <typename, typename T>
struct has_size {
    static_assert(std::integral_constant<T, false>::value,
                  "Second template parameter needs to be of function type.");
};

// specialization that does the checking
template <typename C, typename Ret, typename... Args>
struct has_size<C, Ret(Args...)> {
 private:
    template <typename T>
    static constexpr auto check(T *) ->
        typename std::is_same<decltype(std::declval<T>().size(std::declval<Args>()...)),
                              Ret       // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                              >::type;  // attempt to call it and see if the return type is correct

    template <typename>
    static constexpr std::false_type check(...);

    typedef decltype(check<C>(0)) type;

 public:
    static constexpr bool value = type::value;
};

template<typename T, typename = typename std::enable_if<has_size<T, std::size_t(void)>::value>::type>
int64_t get_size(const T& c) {
    return c.size();
}

template<typename T>
int64_t get_size(T&& c) {
    return -1;
}

int main() {
    std::vector<int> arr{1,2,3};
    cout << get_size(arr) << endl;

    return 0;
}
