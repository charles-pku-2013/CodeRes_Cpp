#include <boost/algorithm/string.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>

#define SPACES      " \t\f\r\v\n"

using namespace std;

int main()
{
    string s("I am studying in Peking University");

    vector<string> words;
    boost::split(words, s, boost::is_any_of(SPACES));

    copy(words.begin(), words.end(), ostream_iterator<string>(cout, "\n"));

    return 0;
}

