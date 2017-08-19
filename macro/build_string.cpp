#include <iostream>
#include <sstream>
#include <string>


#define BUILD_STRING(args)  \
        (static_cast<std::ostringstream&>(std::ostringstream()  \
                  << args << std::flush)).str()


int main()
{
    using namespace std;

    int i = 3, j = 4;

    string str = BUILD_STRING("i = " << i << ", j = " << j);
    cout << str << endl;

    return 0;
}


