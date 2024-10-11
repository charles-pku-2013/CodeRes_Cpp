#include <iostream>
#include <bitset>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

bool ip2bit(const string& ip, bitset<32>& bits) {
    // cout << ip << endl;
    stringstream ss(ip);
    string seg;
    vector<string> arr;
    while (getline(ss, seg, '.')) {
        // cout << seg << endl;
        arr.emplace_back(std::move(seg));       
    }
    if (arr.size() != 4)
    { return false; }

    string str_result;
    for (string& part : arr) {
        int n = stoi(part);
        // cout << n << endl;
        if (n > 255 || n < 0) 
        { return false; }
        bitset<8> b_part(n); // NOTE!!! ipv4地址分4段，每段8位
        // cout << b_part.to_string() << endl;
        str_result.append(b_part.to_string());
        // cout << str_result << endl;
    }

    bitset<32> result(str_result);
    // cout << result.to_string() << endl;
    bits = std::move(result);
    return true;
}

int main() {
    string netmask, ip1, ip2;
    getline(cin, netmask);
    getline(cin, ip1);
    getline(cin, ip2);

    bitset<32> bit_netmask, bit_ip1, bit_ip2;

    if (!ip2bit(netmask, bit_netmask) ||
        !ip2bit(ip1, bit_ip1) ||
        !ip2bit(ip2, bit_ip2)) {
        cout << "1" << endl;
        return 0;
    }

    // check netmask
    string str_bit_netmask = bit_netmask.to_string();
    if (!std::is_partitioned(str_bit_netmask.begin(), str_bit_netmask.end(),  // NOTE
            [](char ch){
            return ch == '1';
        })) {
        // cout << "invalid netmask " << netmask << " " << bit_netmask.to_string() << endl;
        cout << "1" << endl;
        return 0;
    }

    bitset<32> result1 = bit_netmask & bit_ip1;
    bitset<32> result2 = bit_netmask & bit_ip2;
    if (result1 == result2) {
        cout << "0" << endl;
    } else {
        cout << "2" << endl;
    }
}












// 64 位输出请用 printf("%lld")
