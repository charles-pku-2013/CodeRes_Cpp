#include <iostream>
#include <boost/pending/lowest_bit.hpp>
#include <bitset>
#include <string>
#include <climits>

using namespace std;

int main()
{
    cout << sizeof(long long) << endl; // 不能超过8字节 64位
    // empty constructor
    std::bitset<8> b1; // [0,0,0,0,0,0,0,0]

    // unsigned long long constructor
    std::bitset<8> b2(42);          // [0,0,1,0,1,0,1,0]
    // cout << boost::lowest_bit(b2.to_ullong()) << endl;
    // return 0;
    std::bitset<70> bl(ULLONG_MAX); // [0,0,0,0,0,0,1,1,1,...,1,1,1] in C++11
    // cout << boost::lowest_bit(bl.to_ullong()) << endl;
    // return 0;
    std::bitset<8> bs(0xfff0);      // [1,1,1,1,0,0,0,0]
    // cout << boost::lowest_bit(bs.to_ullong()) << endl;
    // return 0;

    // string constructor
    std::string bit_string = "110010";
    std::bitset<8> b3(bit_string);       // [0,0,1,1,0,0,1,0]
    std::bitset<8> b4(bit_string, 2);    // [0,0,0,0,0,0,1,0]
    std::bitset<8> b5(bit_string, 2, 3); // [0,0,0,0,0,0,0,1]

    std::string bit_string2(80, '0');
    bit_string2[0] = '1';
    std::bitset<80> b10(bit_string2);
    cout << boost::lowest_bit(b10.to_ullong()) << endl; // NOTE!!! 会跑出overflow异常
    return 0;

    // string constructor using custom zero/one digits
    std::string alpha_bit_string = "aBaaBBaB";
    std::bitset<8> b6(alpha_bit_string, 0, alpha_bit_string.size(),
                      'a', 'B');         // [0,1,0,0,1,1,0,1]

    // char* constructor using custom digits
    std::bitset<8> b7("XXXXYYYY", 8, 'X', 'Y'); // [0,0,0,0,1,1,1,1]

    std::cout << b1 << '\n' << b2 << '\n' << bl << '\n' << bs << '\n'
              << b3 << '\n' << b4 << '\n' << b5 << '\n' << b6 << '\n'
              << b7 << '\n';

    return 0;
}
