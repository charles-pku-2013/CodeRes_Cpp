#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

int main()
{
    std::ostringstream oss;

    // Make clog use the buffer from oss
	// replace buffer 替换 io buffer
    std::streambuf *former_buff =
        std::clog.rdbuf(oss.rdbuf());

    std::clog << "This will appear in oss!" << std::flush; // flush required

    std::cout << oss.str() << '\n';

    // Give clog back its previous buffer. restore buffer
    std::clog.rdbuf(former_buff);

    return 0;
}



