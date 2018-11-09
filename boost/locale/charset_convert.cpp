/*
 * c++ -o /tmp/test test.cpp -std=c++11 -lboost_locale-mt -g
 * https://kheresy.wordpress.com/2012/03/12/encoding_convert_by_boost_locale/
 * https://www.boost.org/doc/libs/1_67_0/libs/locale/doc/html/charset_handling.html
 */
#include <iostream>
#include <string>
#include <cwchar>
#include <clocale>
#include <boost/locale.hpp>
#include <boost/format.hpp>

using namespace std;

int test1()
{
    //std::locale::global(std::locale("utf-8"));
    std::locale::global(std::locale(""));  // 设置全局的C运行库locale  可以针对cout  fstream等单独设置 空表示默认使用当前系统字符集
    std::string strGbk = "天朝的方式开发刻录机卡洛斯分123qwerwer繫";
    std::wstring wstr = boost::locale::conv::to_utf<wchar_t>(strGbk, "utf8");
    std::string strA = boost::locale::conv::from_utf(wstr, "utf8");
    std::cout << strGbk << std::endl;
    std::wcout << wstr << std::endl;
    std::cout << strA.c_str() << std::endl;
    try
    {
        std::string strBit5;
        strBit5 = boost::locale::conv::between(strA, "big5", "gbk");
        strA = boost::locale::conv::between(strBit5, "GBK", "BIG5");
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "..." << std::endl;
    }
    
    return 0;
}

void print_mb(const char* ptr)
{
    std::mbstate_t state = std::mbstate_t(); // initial state
    const char* end = ptr + std::strlen(ptr);
    int len;
    wchar_t wc;
    while((len = std::mbrtowc(&wc, ptr, end-ptr, &state)) > 0) {
        std::wcout << "Next " << len << " bytes are the character " << wc << '\n';
        ptr += len;
    }
}

int n_octets(unsigned char lb) {
    if (( lb & 0x80 ) == 0 )          // lead bit is zero, must be a single ascii
        return 1;
    else if (( lb & 0xE0 ) == 0xC0 )  // 110x xxxx
        return 2;
    else if (( lb & 0xF0 ) == 0xE0 ) // 1110 xxxx
        return 3;
    else if (( lb & 0xF8 ) == 0xF0 ) // 1111 0xxx
        return 4;
    else
        return -1;
}

#if 0
wchar_t to_wchar(const char *_src, int len)
{
    char src[5];
    wchar_t dst[2] = {L'\0'};
    std::mbstate_t state = std::mbstate_t();

    strncpy(src, _src, len);
    src[4] = '\0';
    const char *pSrc = src;

    // std::size_t mbsrtowcs( wchar_t* dst,
                       // const char** src,
                       // std::size_t len,
                       // std::mbstate_t* ps );
    len = std::mbsrtowcs(nullptr, &pSrc, 0, &state);
    cout << len << endl;
    std::size_t retval = std::mbsrtowcs(dst, &pSrc, 2, &state);
    if (static_cast<std::size_t>(-1) == retval) {
        cout << "convert fail!" << endl;
        dst[0] = L'\0';
    }
    cout << boost::format("%04x") % dst[0] << endl;
    cout << boost::format("%04x") % dst[1] << endl;
    return dst[0];
}
#endif

/*
 * https://en.cppreference.com/w/cpp/string/multibyte
 */
wchar_t to_wchar(const char *src, int len)
{
    wchar_t wch = L'\0';
    std::mbstate_t state = std::mbstate_t(); // initial state
    
    int iLen = std::mbrtowc(&wch, src, len, &state);
    if (iLen <= 0) {
        wch = L'\0';
    }
    // wcout << wch << endl;
    return wch;
}

// wchar_t to_wchar(const char *_src, int len)
// {
    // wstring wstr = boost::locale::conv::to_utf<wchar_t>(_src, _src + len, "utf8");
    // return wstr[0];
// }

int main()
{
    do {
        // std::setlocale(LC_ALL, "zh_CN.UTF-8");
        std::locale::global(std::locale("")); // NOTE!!! 必须有
        string s = "丰";
        int len = n_octets(s[0]);
        cout << len << endl;
        if (len > 0) {
            wchar_t wch = to_wchar(s.c_str(), len);
            cout << boost::format("%04x") % wch << endl;
        }
    } while (0);

    // wchar_t wch = L'\u4e30';
    // cout << sizeof(wch) << endl;
    // wcout << wch << endl;
    // do {
        // wstring wstr = L"丰";
        // wcout << wstr << endl;
        // cout << sizeof(wstr[0]) << endl;
        // cout << boost::format("%04x") % wstr[0] << endl; // 4e30 
        // wchar_t wch = wstr[0];
        // uint8_t *p = (uint8_t*)&wch;
        // for (int i = 0; i < sizeof(wch); ++i) {
            // cout << boost::format("%02x ") % p[i];
        // }
        // cout << endl;
    // } while (0);
    
    // do {
        // wchar_t wch = L'丰';
        // cout << boost::format("%04x") % wch << endl;
        // string s = "丰";
        // const wchar_t *pwstr = (const wchar_t*)(s.c_str());
        // cout << boost::format("%04x") % pwstr[0] << endl; // b0b8e4 NOTE!!! 不能直接用强制类型转换
        // cout << n_octets(s[0]) << endl;
    // } while (0);

    return 0;
}
