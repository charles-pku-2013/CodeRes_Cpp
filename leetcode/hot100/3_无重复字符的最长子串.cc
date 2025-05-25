/*
 * https://leetcode.cn/problems/longest-substring-without-repeating-characters/description/?envType=problem-list-v2&envId=2cktkvj
 */

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        // 哈希集合，记录每个字符是否出现过
        unordered_set<char> occ;
        int n = s.size();
        // 右指针，初始值为 -1，相当于我们在字符串的左边界的左侧，还没有开始移动
        int rk = -1, ans = 0;
        // 枚举左指针的位置，初始值隐性地表示为 -1
        for (int i = 0; i < n; ++i) {
            if (i != 0) {
                // 左指针向右移动一格，移除一个字符
                occ.erase(s[i - 1]);
            }
            while (rk + 1 < n && !occ.count(s[rk + 1])) {
                // 不断地移动右指针
                occ.insert(s[rk + 1]);
                ++rk;
            }
            // 第 i 到 rk 个字符是一个极长的无重复字符子串
            ans = max(ans, rk - i + 1);
        }
        return ans;
    }

    int lengthOfLongestSubstring(string s) {
        int len = s.length();
        if (len <= 1) {
            return len;
        }

        auto check = [](const std::string& str)->bool {
            std::unordered_set<char> set(str.begin(), str.end());
            return str.length() == set.size();
        };

        while (len > 1) {
            for (int i = 0; i <= s.length() - len; ++i) {
                auto substr = s.substr(i, len);
                if (check(substr)) {
                    return substr.length();
                }
            }
            --len;
        }

        return len;
    }
};
