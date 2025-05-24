#if 0
https://leetcode.cn/problems/find-all-anagrams-in-a-string/description/?envType=problem-list-v2&envId=2cktkvj
给定两个字符串 s 和 p，找到 s 中所有 p 的 异位词 的子串，返回这些子串的起始索引。不考虑答案输出的顺序。

示例 1:
输入: s = "cbaebabacd", p = "abc"
输出: [0,6]
解释:
起始索引等于 0 的子串是 "cba", 它是 "abc" 的异位词。
起始索引等于 6 的子串是 "bac", 它是 "abc" 的异位词。

示例 2:
输入: s = "abab", p = "ab"
输出: [0,1,2]
解释:
起始索引等于 0 的子串是 "ab", 它是 "ab" 的异位词。
起始索引等于 1 的子串是 "ba", 它是 "ab" 的异位词。
起始索引等于 2 的子串是 "ab", 它是 "ab" 的异位词。
#endif

class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        std::size_t sLen = s.length();
        std::size_t pLen = p.length();

        std::vector<int> res;

        if (sLen < pLen || pLen == 0) {
            return res;
        }

        std::unordered_set<std::string> dict;

        do {
            dict.insert(p);
        } while (std::next_permutation(p.begin(), p.end()));

        // NOTE i <= sLen
        for (std::size_t i = pLen; i <= sLen; ++i) {
            std::size_t j = i - pLen;
            if (dict.count(s.substr(j, pLen))) {
                res.emplace_back(j);
            }
        }

        return res;
    }
};
