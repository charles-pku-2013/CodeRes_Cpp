#include <iostream>
#include <type_traits>
#include <vector>
#include <map>

template<typename T, typename = void>
struct is_iterator
{
   static constexpr bool value = false;
};

// template<typename T>
// struct is_iterator<T, typename std::enable_if<!std::is_same<typename std::iterator_traits<T>::value_type, void>::value>::type>
// {
   // static constexpr bool value = true;
// };
template<typename T>
struct is_iterator<T, typename std::enable_if<!std::is_same<typename T::value_type, void>::value>::type>
{
   static constexpr bool value = true;
};

template<typename T>
void AddRecord(const std::string& key, const T& val) {
    std::cout << "1" << std::endl;
}

template<typename Iter, typename = typename std::enable_if<!std::is_same<typename Iter::value_type, void>::value>::type>
void AddRecord(Iter beg, Iter end) {
   static_assert(is_iterator<Iter>::value);
    std::cout << "2" << std::endl;
}

// template<typename Iter>
// void AddRecord(Iter beg, Iter end, const std::string& prefix) {
    // std::cout << "3" << std::endl;
// }

template<typename... Args>
void AddRecordHelper(Args&&... args) {
    AddRecord(std::forward<Args>(args)...);
}

int main()
{
   static_assert(!is_iterator<int>::value);
   // static_assert(is_iterator<int*>::value);
   static_assert(is_iterator<std::vector<int>::iterator>::value);
   static_assert(is_iterator<std::map<std::string, std::string>::iterator>::value);

   AddRecordHelper("Name", "charles");
   return 0;
}
