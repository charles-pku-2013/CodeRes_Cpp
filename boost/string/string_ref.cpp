#include <iostream>
#include <boost/utility/string_ref.hpp>

/*
 * valgrind --tool=memcheck --leak-check=full /tmp/test
 */

/*
 * total heap usage: 2 allocs, 2 frees, 60 bytes allocated
 */
// std::string extract_part(const std::string &bar) 
// { return bar.substr( 2, 3 ); }


/*
 * total heap usage: 0 allocs, 0 frees, 0 bytes allocated
 */
boost::string_ref extract_part(boost::string_ref bar) 
{ return bar.substr( 2, 3 ); }

int main()
{
    using namespace std;

    if (extract_part("ABCDEFG").front() == 'C')
        cout << "Found!" << endl;

    return 0;
}


#if 0

// constructor
BOOST_CONSTEXPR basic_string_ref ();    // Constructs an empty string_ref
BOOST_CONSTEXPR basic_string_ref(const charT* str); // Constructs from a NULL-terminated string
BOOST_CONSTEXPR basic_string_ref(const charT* str, size_type len); // Constructs from a pointer, length pair
template<typename Allocator>
basic_string_ref(const std::basic_string<charT, traits, Allocator>& str); // Constructs from a std::string
basic_string_ref (const basic_string_ref &rhs);
basic_string_ref& operator=(const basic_string_ref &rhs);

All iterators are const_iterators

BOOST_CONSTEXPR const charT& operator[](size_type pos) const ;
const charT& at(size_t pos) const ;
BOOST_CONSTEXPR const charT& front() const ;
BOOST_CONSTEXPR const charT& back()  const ;
BOOST_CONSTEXPR const charT* data()  const ;

Modifying the string_ref (but not the underlying data):
void clear();
void remove_prefix(size_type n);
void remove_suffix(size_type n);

BOOST_CONSTEXPR basic_string_ref substr(size_type pos, size_type n=npos) const ; // Creates a new string_ref
bool starts_with(charT c) const ;
bool starts_with(basic_string_ref x) const ;
bool ends_with(charT c) const ;
bool ends_with(basic_string_ref x) const ;

#endif
