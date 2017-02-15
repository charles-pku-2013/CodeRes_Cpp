// file:////doc/html/string_algo/quickref.html

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string_regex.hpp>       // optional

#include <boost/lexical_cast.hpp>
boost::lexical_cast<type>(str)          // 错误跑出异常 bad_lexical_cast : std::bad_cast
boost::conversion::try_lexical_convert(str, dstVal)  // return true on success false on fail

to_upper
trim
to_lower_copy
ireplace_first_copy
iends_with
(all( text1, is_lower() )
is_from_range('a','z')
is_digit()
trim_left
trim_right
trim_left_copy
trim_right_copy
trim_left_if(phone,is_any_of("0"));
boost::trim_if(key, boost::is_any_of("\"" SPACES));
nRead = boost::lexical_cast<size_t>(v.value);
boost::conversion::try_lexical_convert(v.value, nRead)

void find()
{
    char text[]="hello dolly!";
    iterator_range<char*> result=find_last(text,"ll");

    transform( result.begin(), result.end(), result.begin(), bind2nd(plus<char>(), 1) );
    // text = "hello dommy!"            

    to_upper(result); // text == "hello doMMy!"

    // iterator_range is convertible to bool
    if(find_first(text, "dolly"))
        cout << "Dolly is there" << endl;
}

void replace()
{
    string str1="Hello  Dolly,   Hello World!";
    replace_first(str1, "Dolly", "Jane");      // str1 == "Hello  Jane,   Hello World!"
    string newS = replace_first_copy(...);
    replace_last(str1, "Hello", "Goodbye");    // str1 == "Hello  Jane,   Goodbye World!"
    erase_all(str1, " ");                      // str1 == "HelloJane,GoodbyeWorld!"
    erase_head(str1, 6);                       // str1 == "Jane,GoodbyeWorld!"
}

void find_iterator()
{
    string str1("abc-*-ABC-*-aBc");
    // Find all 'abc' substrings (ignoring the case)
    // Create a find_iterator
    typedef find_iterator<string::iterator> string_find_iterator;
    for(string_find_iterator It =
            make_find_iterator(str1, first_finder("abc", is_iequal()));
            It != string_find_iterator(); ++It)
    {
        cout << copy_range<std::string>(*It) << endl;
    }

    // Output will be:
    // abc
    // ABC
    // aBC

    typedef split_iterator<string::iterator> string_split_iterator;
    for(string_split_iterator It =
            make_split_iterator(str1, first_finder("-*-", is_iequal()));
            It != string_split_iterator(); ++It)
    {
        cout << copy_range<std::string>(*It) << endl;
    }

    // Output will be:
    // abc
    // ABC
    // aBC
}

void split()
{
    string str1("hello abc-*-ABC-*-aBc goodbye");

    typedef vector< iterator_range<string::iterator> > find_vector_type;

    find_vector_type FindVec; // #1: Search for separators
    ifind_all( FindVec, str1, "abc" ); // FindVec == { [abc],[ABC],[aBc] }

    typedef vector< string > split_vector_type;

    split_vector_type SplitVec; // #2: Search for tokens
    split( SplitVec, str1, is_any_of("-*"), token_compress_on ); 
    // SplitVec == { "hello abc","ABC","aBc goodbye" }
}




