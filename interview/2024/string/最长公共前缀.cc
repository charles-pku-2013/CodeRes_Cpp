/*
https://www.nowcoder.com/practice/28eb3175488f4434a4a6207f6f484f47?tpId=196&tqId=37086&rp=1&ru=/exam/intelligent&qru=/exam/intelligent&sourceUrl=%2Fexam%2Fintelligent%3FdayCountBigMember%3D%25E8%25BF%259E%25E7%25BB%25AD%25E5%258C%2585%25E6%259C%2588%26questionJobId%3D10%26tagId%3D21003%26type%3Dvip&difficulty=undefined&judgeStatus=undefined&tags=&title=
https://leetcode.cn/problems/longest-common-prefix/description/
 */

// 横向比较
class Solution {
public:
    // 横向
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) { return ""; }
        if (strs.size() == 1) { return strs[0]; }

        auto common_prefix = [](const string& lhs, const string& rhs)->string {
            if (lhs.empty() || rhs.empty())
            { return ""; }

            std::size_t i = 0;
            for (; i < lhs.length() && i < rhs.length() && lhs[i] == rhs[i]; ++i);

            return lhs.substr(0, i);
        };

        string result = strs[0];

        for (std::size_t i = 1; i < strs.size(); ++i) {
            result = common_prefix(result, strs[i]);
        }

        return result;
    }

    // 纵向
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) { return ""; }
        if (strs.size() == 1) { return strs[0]; }

        auto is_equal = [&](std::size_t i)->bool {
            auto& first = strs[0];

            if (i >= first.length()) {
                return false;
            }

            char ch = first[i];
            for (std::size_t j = 1; j < strs.size(); ++j) {
                auto& s = strs[j];
                if (i >= s.length() || s[i] != ch) {
                    return false;
                }
            }

            return true;
        };

        std::size_t i = 0;
        for (; is_equal(i); ++i);

        return strs[0].substr(0, i);
    }
};

