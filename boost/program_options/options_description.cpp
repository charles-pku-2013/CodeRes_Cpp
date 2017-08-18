// Copyright Vladimir Prus 2002-2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

/*
 * default_value() is the value that will be put in the variables_map if the user didn't specify another value:
 * 
 * ./a.out             # implies width=75 if that's the default_value for width
 * ./a.out --width=80  # default_value not used
 * implicit_value() is the value that will be used if the user specifies the option but without an adjacent value.
 * 
 * ./a.out --width     # implies width=75 if that's the implicit_value for width
 * ./a.out --width=80  # implicit value not used
 * If you use implicit_value then in commandline options's short options the user must specify the value immediately after the option:
 * 
 * ./a.out -w80   # implicit_value not used
 * ./a.out -w 80  # wrong: 80 parsed as extra arg if implicit_value is defined
 */

#include <boost/program_options.hpp>

using namespace boost;
namespace po = boost::program_options;

#include <iostream>
#include <algorithm>
#include <iterator>
using namespace std;


// A helper function to simplify the main part.
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(os, " ")); 
    return os;
}

int main(int ac, char* av[])
{
    try {
        int opt;
        int portnum;
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("optimization", po::value<int>(&opt)->default_value(10), 
                  "optimization level")
            ("verbose,v", po::value<int>()->implicit_value(1),
                  "enable verbosity (optionally specify level)")
            ("listen,l", po::value<int>(&portnum)->implicit_value(1001)
                  ->default_value(0,"no"),
                  "listen on a port.")
            ("include-path,I", po::value< vector<string> >(), 
                  "include path")
            ("input-file", po::value< vector<string> >(), "input file")
        ;

/*
 * The command line tokens which have no option name, as above, 
 * are called "positional options" by this library. They can be handled too. 
 * With a little help from the user, the library can decide that "a.cpp" 
 * really means the same as "--input-file=a.cpp". Here's the additional code we need:
 */
/*
 * The positional_options_description class allows the command line parser to assign the names. 
 * The class specifies how many positional options are allowed, and for each allowed option, 
 * specifies the name. For example:
 * 
 * 🍑🍑positional_options_description pd; pd.add("input-file", 1);
 * specifies that for exactly one, first, positional option the name will be "input-file".
 * 
 * It's possible to specify that a number, or even all positional options, be given the same name.
 * 
 * positional_options_description pd;
 * pd.add("output-file", 2).add("input-file", -1);
 * In the above example, the first two positional options will be associated 
 * with name "output-file", and any others with the name "input-file".
 */
        po::positional_options_description p;
        p.add("input-file", -1);
        
        po::variables_map vm;
        po::store(po::command_line_parser(ac, av).
                  options(desc).positional(p).run(), vm);
        po::notify(vm);
    
        if (vm.count("help")) {
            cout << "Usage: options_description [options]\n";
            cout << desc;
            return 0;
        }

        if (vm.count("include-path"))
        {
            cout << "Include paths are: " 
                 << vm["include-path"].as< vector<string> >() << "\n";
        }

        if (vm.count("input-file"))
        {
            cout << "Input files are: " 
                 << vm["input-file"].as< vector<string> >() << "\n";
        }

        if (vm.count("verbose")) {
            cout << "Verbosity enabled.  Level is " << vm["verbose"].as<int>()
                 << "\n";
        }

        cout << "Optimization level is " << opt << "\n";                

        cout << "Listen port is " << portnum << "\n";                
    }
    catch(std::exception& e)
    {
        cout << e.what() << "\n";
        return 1;
    }    
    return 0;
}
