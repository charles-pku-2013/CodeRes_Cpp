
#if 0
In Example 2, matches is a sequence of sub_match objects. The sub_match class has the members first and second, which are iterators which refer to the first and one-past-the-last elements in the original string. matches[0] contains the entire original string, and the sub_match objects at indexes matches[1...n] each refer to the substrings n that match the corresponding subexpression in the original expression.
A subexpression is a part of the original regular expression that is contained within parentheses. For example, this regular expression has three subexpressions:
(\d{1,2})\/(\d{1,2})\/(\d{2}|\d{4})

Expression: (ftp|http|https):\/\/((\w+\.)*(\w*))\/([\w\d]+\/{0,1})+
String:     http://www.foo.com/bar
        matches[0] = http://www.foo.com/bar
        matches[1] = http
        matches[2] = www.foo.com
        matches[3] = foo.
        matches[4] = com
        matches[5] = bar
#endif


#include <iostream>
#include <string>
#include <boost/regex.hpp>

using namespace std;

int main( ) {

   std::string s, sre;
   boost::regex re;
   boost::cmatch matches;

   while (true)
   {
      cout << "Expression: ";
      cin >> sre;
      if (sre == "quit")
      {
         break;
      }

      cout << "String:     ";
      cin >> s;

      try
      {
         // Assignment and construction initialize the FSM used
         // for regexp parsing
         re = sre;
      }
      catch (boost::regex_error& e)
      {
         cout << sre << " is not a valid regular expression: \""
              << e.what() << "\"" << endl;
         continue;
      }
      // if (boost::regex_match(s.begin(), s.end(), re))
      if (boost::regex_match(s.c_str(), matches, re))
      {
         // matches[0] contains the original string.  matches[n]
         // contains a sub_match object for each matching
         // subexpression
         for (int i = 1; i < matches.size(); i++)
         {
            // sub_match::first and sub_match::second are iterators that
            // refer to the first and one past the last chars of the
            // matching subexpression
            string match(matches[i].first, matches[i].second);
            cout << "\tmatches[" << i << "] = " << match << endl;
         }
      }
      else
      {
         cout << "The regexp \"" << re << "\" does not match \"" << s << "\"" << endl;
      }
   }
}
