// stringbuf example
#include <iostream>     // std::cout, std::ostream, std::hex
#include <sstream>      // std::stringbuf
#include <string>       // std::string



int main()
{
    using namespace std;

    stringstream ostr;
    int n = 97;
    ostr.write( (char*)&n, sizeof(n) );
    ostr.flush();

    char ch;
    n = 0;
    while( ostr ) {
        ostr.get(ch);
        cout << ch << endl;
        ++n;
    }
    cout << n << endl;              //!! n is 5, stringstream always put 0 in the end

    // int a;
    // ostr.read( (char*)&a, sizeof(n) );
    // if( !ostr )
        // cout << "ostr not in good state" << endl;
    // cout << a << endl;

    // cout << ostr.rdbuf() << endl;
    // stringbuf *buf = ostr.rdbuf();
    // cout << buf->in_avail() << endl;

    return 0;
}




/*
 * int main ()
 * {
 *   std::stringbuf buffer;      // empty stringbuf
 * 
 *   std::ostream os (&buffer);  // associate stream buffer to stream
 * 
 *   // mixing output to buffer with inserting to associated stream:
 *   buffer.sputn ("255 in hexadecimal: ",20);
 *   os << std::hex << 255;
 * 
 *   std::cout << buffer.str() << std::endl;
 *   std::cout << &buffer << std::endl;
 * 
 *   return 0;
 * }
 */


/*
 * // sputn() example
 * #include <iostream>     // std::streambuf
 * #include <fstream>      // std::ofstream
 * 
 * int main () {
 *   const char sentence[]= "Sample sentence";
 * 
 *   std::ofstream ostr ("test.txt");
 *   if (ostr) {
 *     std::streambuf * pbuf = ostr.rdbuf();
 *     pbuf->sputn (sentence,sizeof(sentence)-1);
 *     ostr.close();
 *   }
 * 
 *   return 0;
 * }
 */




/*
 * #include <iostream>
 * #include <sstream>
 *  
 * int main()
 * {
 *     std::ostringstream s1;
 *     std::streamsize sz = s1.rdbuf()->sputn("This is a test", 14);
 *     s1 << '\n';
 *     std::cout << "The call to sputn() returned " << sz << '\n'
 *               << "The output sequence contains " << s1.str();
 *  
 *     std::istringstream s2;
 *     sz = s2.rdbuf()->sputn("This is a test", 14);
 *     std::cout << "The call to sputn() on an input stream returned " << sz << '\n';
 * }
 */
