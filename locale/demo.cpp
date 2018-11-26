#include <iostream>
#include <locale>
 
int main()
{
    std::wcout << "User-preferred locale setting is " << std::locale("").name().c_str() << '\n';
    // 在启动时，全局本地环境是 "C" 本地环境
    std::wcout << 1000.01 << '\n';
    // 以用户偏好的本地环境替换 C++ 本地环境和 C 本地环境
    std::locale::global(std::locale(""));
    // 将来的宽字符输出使用新的全局本地环境
    std::wcout.imbue(std::locale());
    // 再次输出同一数字
    std::wcout << 1000.01 << '\n';
}
