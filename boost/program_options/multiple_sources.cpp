/*
 * c++ -o /tmp/test multiple_sources.cpp -lboost_program_options -std=c++11 -g
 */
// Copyright Vladimir Prus 2002-2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

/* Shows how to use both command line and config file. */

// 命令行参数会覆盖配置文件

#include <boost/program_options.hpp>
namespace po = boost::program_options;


#include <iostream>
#include <fstream>
#include <iterator>
using namespace std;

// A helper function to simplify the main part.
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(os, " "));
    return os;
}

/*
 * composing: values from different sources should be composed.
 * Otherwise, value from the first source is used and values from
 * other sources are discarded.
 */
int main(int ac, char* av[])
{
    try {
        int opt;
        string config_file;

        // Declare a group of options that will be
        // allowed only on command line
        po::options_description generic("Generic options");
        generic.add_options()
            ("version,v", "print version string")
            ("help", "produce help message")
            ("config,c", po::value<string>(&config_file)->default_value("multiple_sources.cfg"),
                  "name of a file of a configuration.")
            ;

        // Declare a group of options that will be
        // allowed both on command line and in
        // config file
        po::options_description config("Configuration");
        config.add_options()
            ("optimization", po::value<int>(&opt)->default_value(10),
                  "optimization level")
            ("include-path,I",
                 // po::value< vector<string> >()->composing(),
                 po::value< vector<string> >()->multitoken(), // 💜💜 参数列表指定方式：/tmp/test -I/usr -I/var -I/opt
                 "include path")
            ;

        // Hidden options, will be allowed both on command line and
        // in config file, but will not be shown to the user.
        po::options_description hidden("Hidden options");
        hidden.add_options()
            ("input-file", po::value< vector<string> >(), "input file")
            ;


        po::options_description cmdline_options;
        cmdline_options.add(generic).add(config).add(hidden);

        po::options_description config_file_options;
        config_file_options.add(config).add(hidden);

        po::options_description visible("Allowed options");
        visible.add(generic).add(config);

        po::positional_options_description p;
        p.add("input-file", -1);

        po::variables_map vm;
        store(po::command_line_parser(ac, av).
              options(cmdline_options).positional(p).run(), vm);
        notify(vm);

        ifstream ifs(config_file.c_str());
        if (!ifs)
        {
            cout << "can not open config file: " << config_file << "\n";
            return 0;
        }
        else
        {
            po::store(po::parse_config_file(ifs, config_file_options), vm);
            po::notify(vm);
        }

        if (vm.count("help")) {
            cout << visible << "\n";
            return 0;
        }

        if (vm.count("version")) {
            cout << "Multiple sources example, version 1.0\n";
            return 0;
        }

        if (vm.count("include-path"))
        {
            cout << "Include paths are: "
                 << vm["include-path"].as< vector<string> >() << "\n";
            const vector<string> &paths = vm["include-path"].as< vector<string> >();
            cout << "include-path size = " << paths.size() << endl;
        }

        if (vm.count("input-file"))
        {
            cout << "Input files are: "
                 << vm["input-file"].as< vector<string> >() << "\n";
        }

        cout << "Optimization level is " << opt << "\n";
    }
    catch(exception& e)
    {
        cout << e.what() << "\n";
        return 1;
    }


    return 0;
}
