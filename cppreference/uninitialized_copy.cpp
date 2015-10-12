#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>


template<class InputIt, class ForwardIt>
ForwardIt uninitialized_copy(InputIt first, InputIt last, ForwardIt d_first)
{
    typedef typename std::iterator_traits<ForwardIt>::value_type Value;
    ForwardIt current = d_first;
    try {
        for (; first != last; ++first, ++current) {
            ::new (static_cast<void*>(std::addressof(*current))) Value(*first);	//!! addressof and placement new
        }
        return current;
    } catch (...) {
        for (; d_first != current; ++d_first) {
            d_first->~Value();
        }
        throw;	//!! re-throw exception
    }
}

 
int main()
{
    std::vector<std::string> v = {"This", "is", "an", "example"};
 
    std::string* p;
    std::size_t sz;
    std::tie(p, sz)  = std::get_temporary_buffer<std::string>(v.size());
    sz = std::min(sz, v.size());
 
    std::uninitialized_copy(v.begin(), v.begin() + sz, p);
 
    for (std::string* i = p; i != p+sz; ++i) {
        std::cout << *i << ' ';
        i->~basic_string<char>();
    }
    std::return_temporary_buffer(p);
}












