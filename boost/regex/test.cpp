#include <iostream>
#include <stdlib.h>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

int main()
{
    string strRegex;
    boost::regex re;

    getline(cin, strRegex);
    cout << "regex is: " << strRegex << endl;
    try {
        // Set up the regular expression for case-insensitivity
        re.assign(strRegex);
    } catch (boost::regex_error& e) {
        cout << strRegex << " is not a valid regular expression: \"" << e.what() << "\"" << endl;
        return 0;
    }

    string line;
    while (getline(cin, line)) {
        boost::smatch   matches;
        if (boost::regex_match(line, matches, re)) {
            cout << "match!" << endl;
            for (auto it = matches.begin(); it != matches.end(); ++it)
                cout << *it << endl;
        } else {
            cout << "Not match!" << endl;
        } // if
    } // while

    return 0;
}



#if 0
.*func_name\[(.*)\]\,serialize\s+reqdata\[(\d+)\,\s*(.*)\].*
[notic](queryserver.cpp,058):start enter MTMultiSearch;    [fun=MTMultiSearch][notic](queryserver.cpp,206):func_name[MTMultiSearch],serialize reqdata[1066,0B000100000015E9AB98E696B0E4B887E8BEBEE5BA97E781ABE994850B00020000001122E6B500];
#endif
