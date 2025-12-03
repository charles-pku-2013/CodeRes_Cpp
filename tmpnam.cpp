#include <iostream>
#include <cstdio>
#include <string>
 
int main()
{
    std::string name1 = std::tmpnam(nullptr);
    std::cout << "temporary file name: " << name1 << '\n';
 
    char name2[L_tmpnam];
    std::cout << L_tmpnam << std::endl;
    if (std::tmpnam(name2)) {
        std::cout << "temporary file name: " << name2 << '\n';
    }

    return 0;
}
