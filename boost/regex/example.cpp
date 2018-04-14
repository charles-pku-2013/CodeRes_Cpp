#if 0
http://www.onlamp.com/pub/a/onlamp/2006/04/06/boostregex.html?page=1

Symbol	Meaning
c	Match the literal character c once, unless it is one of the special characters.
^	Match the beginning of a line.
.	Match any character that isn't a newline.
$	Match the end of a line.
|	Logical OR between expressions.
()	Group subexpressions.
[]	Define a character class.
*	Match the preceding expression zero or more times.
+	Match the preceding expression one ore more times.
?	Match the preceding expression zero or one time.
{n}	Match the preceding expression n times.
{n,}	Match the preceding expression at least n times.
{n, m}	Match the preceding expression at least n times and at most m times.
\d	Match a digit.
\D	Match a character that is not a digit.
\w	Match an alpha character, including the underscore.
\W	Match a character that is not an alpha character.
\s	Match a whitespace character (any of \t, \n, \r, or \f).
\S	Match a non-whitespace character.
\t	Tab.
\n	Newline.
\r	Carriage return.
\f	Form feed.
\m	Escape m, where m is one of the metacharacters described above: ^, ., $, |, (), [], *, +, ?, \, or /.
#endif


#include <iostream>
#include <stdlib.h>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

int main()
{
    // This regex is compiled at start-up and matches YYYY-MM-DD dates. If it
    // contains a syntax error, the program aborts at start-up with an
    // exception.
    static const boost::regex
        date_regex("(199[0-9]|200[0-9])-([1-9]|0[1-9]|1[012])-([1-9]|[0-2][1-9]|3[01])");

    // First example: char* c-style input strings use boost::cmatch results.
    {
        const char *input_cstr = "2007-03-14";
        boost::cmatch char_matches;

        if (boost::regex_match(input_cstr, char_matches, date_regex))
        {
            // Convert the parsed number using boost's lexical_cast library
            int year = boost::lexical_cast<int>( char_matches[1] ); // 匹配小括号子表达式
            // Or use the old way: get the std::string object, then it's char*
            int month = atoi( char_matches[2].str().c_str() );

            std::cout << "First example:"
                      << " year " << year
                      << " month " << month
                      << " day " << char_matches[3] << "\n";
        } 
        else
        {
            std::cout << "First example should have matched the regex.\n";
        }
    }

    // Second example: STL strings use boost::smatch results.
    {
        std::string input_stlstr = "2007-03-34";
        boost::smatch str_matches;

        if (boost::regex_match(input_stlstr, str_matches, date_regex))
        {
            std::cout << "Second example shouldn't have matched the regex.\n";
        }
        else
        {
            std::cout << "Second example didn't match the regex. This was intended.\n";
        }
    }

    // Third example: Temporary regex object and no capture results needed.
    {
        if (boost::regex_match("2007", boost::regex("(199[0-9]|200[0-9])")))
        {
            std::cout << "Third example matched the temporary regex object.\n";
        }
        else
        {
            std::cout << "Third example should have matched the regex.\n";
        }
    }

    // Fourth example: regex_match matches the whole string while regex_search
    // matches substrings just like perl.
    {
        std::string input = "Today is 2007-03-14, how are you?";

        if (boost::regex_match(input, date_regex))
        {
            std::cout << "Fourth example (regex_match) shouldn't match.\n";
        }
        else
        {
            std::cout << "As expected, the fourth example (regex_match) didn't match.\n";
        }

        if (boost::regex_search(input, date_regex))
        {
            std::cout << "While the fourth example using regex_search did matched.\n";
        }
        else
        {
            std::cout << "Fourth example using regex_search should have matched the regex.\n";
        }
    }
}
