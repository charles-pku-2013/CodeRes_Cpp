#include <iostream>
#include <fstream>
#include <memory>


inline
std::shared_ptr<std::istream>
open_input(const std::string &fname, std::ios_base::openmode mode = std::ios_base::in)
{
    std::shared_ptr<std::istream> ret;

    if (fname.empty() || fname == "-")
        ret.reset(&std::cin, [](std::istream*){});
    else
        ret.reset(new std::ifstream(fname, mode));

    return ret;
}

inline
std::shared_ptr<std::ostream>
open_output(const std::string &fname, std::ios_base::openmode mode = std::ios_base::out)
{
    std::shared_ptr<std::ostream> ret;

    if (fname.empty() || fname == "-")
        ret.reset(&std::cout, [](std::ostream*){});
    else
        ret.reset(new std::ofstream(fname, mode));

    return ret;
}


int main()
{
    using namespace std;

    auto pOut = open_ostream("");
    *pOut << "Hello world" << endl;

    return 0;
}


