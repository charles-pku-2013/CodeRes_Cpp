/*
 * c++ -o /tmp/test first.cpp -lboost_program_options -std=c++11 -g
 */
// Copyright Vladimir Prus 2002-2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

/* The simplest usage of the library.
 */

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <iterator>
using namespace std;

/*
 * default_value() is the value that will be put in the variables_map if the user didn't specify another value:
 * implicit_value() is the value that will be used if the user specifies the option but without an adjacent value.
 * required 必须提供的参数
 */

// 参数检查参考 validate
#if 0
template<typename T>
void check_range(const T& value, const T& min, const T& max)
{
   if (value < min || value > max)
       // throw exception
}

opt::value<unsigned short>()->default_value(5)->notifier
        (boost::bind(&check_range<unsigned short>, _1, 0, 10));
#endif

int main(int ac, char* av[])
{
    try {

        int count = 0;

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "produce help message")
            ("compression,c", po::value<double>()->default_value(9), "set compression level")
            ("debug", po::value<bool>()->default_value(false), "debug mode")
            ("count,n", po::value<int>(&count)->required(), "set count");   // 设置已有变量

        po::variables_map vm;
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return 0;
        }

        if (vm.count("compression")) {
            cout << "Compression level was set to "
                 << vm["compression"].as<double>() << ".\n";
        } else {
            cout << "Compression level was not set.\n";
        }

        // cout << vm["test"].as<int>() << endl;

        // check compression
        {
            auto it = vm.find("compression");
            if (it == vm.end()) cout << "compression not set!" << endl;
            else cout << "compression set to " << it->second.as<double>() << endl;
        }

        // check debug
        {
            auto it = vm.find("debug");
            if (it != vm.end())
                cout << "debug mode: " << it->second.as<bool>() << endl;
        }

        cout << "count = " << count << endl;

    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }

    return 0;
}
