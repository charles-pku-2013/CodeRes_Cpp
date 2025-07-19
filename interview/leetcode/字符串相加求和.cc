/*
https://leetcode.cn/problems/add-strings/description/
 */
class Solution {
public:
    string addStrings(string num1, string num2) {
        std::reverse(num1.begin(), num1.end());
        std::reverse(num2.begin(), num2.end());

        std::size_t len = num1.length() > num2.length() ? num1.length() : num2.length();
        num1.resize(len, '0');
        num2.resize(len, '0');

        std::string result;
        int flag = 0;
        result.reserve(len + 1);

        auto add_char = [](char a, char b)->int {
            return (a - '0') + (b - '0');
        };

        for (std::size_t i = 0; i < len; ++i) {
            int sum = flag + add_char(num1[i], num2[i]);
            flag = sum / 10;
            sum %= 10;
            result.append(1, sum + '0');
        }

        if (flag) {
            result.append(1, '1');
        }

        std::reverse(result.begin(), result.end());
        return result;
    }
};
