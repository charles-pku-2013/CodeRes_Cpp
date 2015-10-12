#include <boost/locale.hpp>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <ctime>

int main()
{
    using namespace boost::locale;
    using namespace std;
/*
    wchar_t Str[] = L"你好";
 (gdb) x/3xw Str
 0xbffff144:	0x00004f60	0x0000597d	0x00000000      // utf-8 编码
*/
    std::locale loc("zh_CN.UTF-8");
    std::locale::global( loc );
    
    string someString = "hello你好";
    
    locale conv_loc = boost::locale::util::create_info(loc, loc.name());
    string output = boost::locale::conv::to_utf<char>(someString, conv_loc);
    wstring woutput = boost::locale::conv::to_utf<wchar_t>(someString, conv_loc);
    
    cout << someString.length() << endl;
    cout << output.length() << endl;
    
    cout << memcmp(someString.c_str(), output.c_str(), output.length() ) << endl;
    
    cout << someString << endl;
    cout << output << endl;
    wprintf( L"%S\n", woutput.c_str() );
//    wprintf( "%C\n", L'中' );
    cout << woutput.length() << endl;
    cout << woutput.size() << endl;
    cout << sizeof(woutput[0]) << endl;
    
    return 0;
}

/*
 (gdb) x/11xb output.c_str()
 0x804e6a4:	0x68	0x65	0x6c	0x6c	0x6f        // "Hello"
 
            0xe4	0xbd	0xa0
 0x804e6ac:	0xe5	0xa5	0xbd        // "你好" in Python Str = '你好'
            //!! 等同于 u'你好'.encode('UTF-8')      // 以utf-8编码(序列化)后的字节流
 // !! 普通string，英文1字节，非英文3字节
 (gdb) x/28xb woutput.c_str()
 0x8056cb4:	0x68	0x00	0x00	0x00	0x65	0x00	0x00	0x00
 0x8056cbc:	0x6c	0x00	0x00	0x00	0x6c	0x00	0x00	0x00
 0x8056cc4:	0x6f	0x00	0x00	0x00	// "Hello"
 
            0x60	0x4f	0x00	0x00
 0x8056ccc:	0x7d	0x59	0x00	0x00    // "你好" 和 python 编码一样 S = u'你好'
 //!! 原始的unicode数据，应该是字的编号，与编码后的字节流区别
 //!! wstring 都是4字节，显然普通string省空间
 */

/*
 // With Python
 >>> Str = 'hello你好'
 >>> [ hex(ord(c)) for c in Str ]
 ['0x68', '0x65', '0x6c', '0x6c', '0x6f', '0xe4', '0xbd', '0xa0', '0xe5', '0xa5', '0xbd']
 >>> U = Str.decode('utf-8')
 >>> [ hex(ord(c)) for c in U ]
 ['0x68', '0x65', '0x6c', '0x6c', '0x6f', '0x4f60', '0x597d']
 >>> U.encode('utf-8')
 'hello\xe4\xbd\xa0\xe5\xa5\xbd'
 */
















