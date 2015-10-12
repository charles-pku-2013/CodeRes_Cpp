#include <iostream>

#include "boost/fdstream.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


void copyfile (const std::string& infile, const std::string& outfile)
{
    int fdin, fdout;

#ifdef _MSC_VER
    fdin = open( infile.c_str(), O_RDONLY | _O_BINARY);
#else
    fdin = open( infile.c_str(), O_RDONLY);
#endif
    if (fdin < 0) {
        throw "open failed on input file";
    }
    std::cout << "open succeeded on input file\n";

#ifdef _MSC_VER
    fdout = open( outfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_BINARY,
                                   S_IREAD | S_IWRITE );
#else
    fdout = open( outfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC,
                                   S_IREAD | S_IWRITE );
#endif
    if (fdout == -1) {
        throw "open failed on output file";
    }
    std::cout << "open succeeded on output file\n";
    
    // copy all standard input to standard output
    boost::fdistream in(fdin);
    boost::fdostream out(fdout);
    out << in.rdbuf();

    close (fdin);
    close (fdout);
}

int main()
{
    try {
        std::string infile="fdstream.hpp";
        std::string outfile="tmp.out";

        std::cout << "low-level I/O from " << infile
                  << " to " << outfile << std::endl;
        copyfile(infile,outfile);
    }
    catch (const char* s) {
        std::cerr << "EXCEPTION: " << s << std::endl;
    }
}

