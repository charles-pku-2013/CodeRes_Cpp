/*
 * read_into_container.cpp
 */

template < typename T >
std::istream& read_into_container( std::istream &is, T &c )
{
    typedef typename T::value_type value_type;
    std::istream_iterator<value_type> beg(is), eof;
    T ret( beg, eof );
    c.swap(ret);
    return is;
}

