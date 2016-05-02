/*
 * read_from_string.cpp
 */

template < typename T >
bool read_from_string( const char *s, T &value )
{
    if ( strcmp(s, "NULL") == 0 || strcmp(s, "null") == 0 ) {
        value = T();
        return true;
    } // if
    std::stringstream str(s);
    str >> value;
    bool ret = (str.good() || str.eof());
    if ( !ret )
        value = T();
    return ret;
}

