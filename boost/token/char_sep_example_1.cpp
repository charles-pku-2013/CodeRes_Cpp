// char_sep_example_1.cpp
/*
 * This first examples shows how to use char_separator 
 * as a replacement for the strtok() function. 
 * We've specified three character delimiters, and they will not show up as output tokens. 
 * We have not specified any kept delimiters, and by default any empty tokens will be ignored.
 */
#include <iostream>
#include <boost/tokenizer.hpp>
#include <string>

int main()
{
    std::string str = ";;Hello|world||-foo--bar;yow;baz|";
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep("-;|");
    tokenizer tokens(str, sep);
    for (tokenizer::iterator tok_iter = tokens.begin();
            tok_iter != tokens.end(); ++tok_iter)
        std::cout << "<" << *tok_iter << "> ";
    std::cout << "\n";

    return EXIT_SUCCESS;
}

/*
 * <Hello> <world> <foo> <bar> <yow> <baz> 
 */
