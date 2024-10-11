/*
https://leetcode.cn/problems/add-binary/
 */
class Solution {
public:
    string addBinary(string a, string b) {
        bitset<10000> _a(a), _b(b);
        unsigned long long ia = _a.to_ullong();  // build from string
        unsigned long long ib = _b.to_ullong();
        unsigned long long sum = ia + ib;
        bitset<10000> bsum(sum);  // build from num
        string result = bsum.to_string();
        auto pos = result.find_first_not_of('0');  // remove leading meaningless 0
        if (pos != string::npos) {
            result = result.substr(pos);
        } else {
            result.resize(1);  // special: 0 + 0
        }
        return result;
    }
};
