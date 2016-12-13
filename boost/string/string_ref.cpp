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
