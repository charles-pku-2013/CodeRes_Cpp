#include <iostream>
#include <vector>
#include <clocale>
#include <string>
#include <cwchar>
 
using namespace std;

void print_wide(const wchar_t* wstr)
{
    std::mbstate_t state = std::mbstate_t();
    std::size_t len = 1 + std::wcsrtombs(nullptr, &wstr, 0, &state);
    std::vector<char> mbstr(len);
    std::wcsrtombs(&mbstr[0], &wstr, mbstr.size(), &state);
    std::cout << "multibyte string: " << &mbstr[0] << '\n'
              << "Length, including '\\0': " << mbstr.size() << '\n';
}

void print_as_wide(const char* mbstr)
{
    std::mbstate_t state = std::mbstate_t();
    std::size_t len = 1 + std::mbsrtowcs(NULL, &mbstr, 0, &state);
    std::vector<wchar_t> wstr(len);
    std::mbsrtowcs(&wstr[0], &mbstr, wstr.size(), &state);
    std::wcout << "Wide string: " << &wstr[0] << '\n'
               << "The length, including '\\0': " << wstr.size() << '\n';
}
 
void String2Wstring(const std::string &sToMatch, std::wstring *_out) {
    std::wstring &out = *_out;
    std::mbstate_t state = std::mbstate_t();
    const char *pSrc = sToMatch.c_str();

    std::size_t iLen = std::mbsrtowcs(nullptr, &pSrc, 0, &state);
    if (iLen != static_cast<std::size_t>(-1)) {
        out.assign(iLen, L'\0');
        std::mbsrtowcs(&out[0], &pSrc, sToMatch.size(), &state);
    }
}

void Wstring2String(const std::wstring &sToMatch, std::string *_out) {
    std::string &out = *_out;
    std::mbstate_t state = std::mbstate_t();
    const wchar_t *pSrc = sToMatch.c_str();

    std::size_t iLen = std::wcsrtombs(nullptr, &pSrc, 0, &state);
    if (iLen != static_cast<std::size_t>(-1)) {
        out.assign(iLen + 1, '\0');
        std::wcsrtombs(&out[0], &pSrc, sToMatch.size(), &state);
    }
}

std::string Wstring2String(wstring sToMatch) {
    string sResult;
    int iLen = wcstombs( NULL, sToMatch.c_str(), 0 );
    if(iLen>0) {
        char *lpsz = new char[iLen + 1];
        wcstombs( lpsz, sToMatch.c_str(), iLen );
        lpsz[iLen] = '\0';
        sResult.assign(lpsz);
        delete []lpsz;
    } else {
        sResult="";
    }
    return sResult;
}

int main()
{
    // std::setlocale(LC_ALL, "en_US.utf8");

    do {
        std::wstring s = L"京东商城";
        cout << s.size() << endl;
        std::string dst;
        Wstring2String(s, &dst);
        cout << dst.size() << endl;
        cout << dst << endl;
    } while (0);

    do {
        std::wstring s = L"京东商城";
        cout << s.size() << endl;
        std::string dst = Wstring2String(s);
        cout << dst.size() << endl;
        cout << dst << endl;
    } while (0);

    // UTF-8 窄多字节编码
    // const wchar_t* wstr = L"z\u00df\u6c34\U0001d10b"; // 或 L"zß水𝄋"
    // print_wide(wstr);
    // std::string dst;
    // Wstring2String(wstr, &dst);
    // cout << dst << endl;
}
