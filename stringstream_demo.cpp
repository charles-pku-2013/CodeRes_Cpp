#include <iostream>
#include <sstream>

int main()
{
    // default constructor (input/output stream)
    std::stringstream buf1;
    buf1 << 7;
    int n = 0;
    buf1 >> n;
    std::cout << "buf1 = " << buf1.str() << " n = " << n << '\n';
 
    // input stream
    std::istringstream inbuf("-10");
    inbuf >> n;
    std::cout << "n = " << n << '\n';
 
    // std::ostringstream buf2("test", std::ios_base::ate);
    // buf2 << '1';
    /*
     * app seek to the end of stream before each write
     * ate seek to the end of stream immediately after open
     */
    std::ostringstream buf2("test");
    buf2 << 1;      //!! 默认新插入元素在头部
    std::cout << buf2.str() << '\n';

    return 0;
}
