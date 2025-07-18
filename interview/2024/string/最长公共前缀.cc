/*
https://www.nowcoder.com/practice/28eb3175488f4434a4a6207f6f484f47?tpId=196&tqId=37086&rp=1&ru=/exam/intelligent&qru=/exam/intelligent&sourceUrl=%2Fexam%2Fintelligent%3FdayCountBigMember%3D%25E8%25BF%259E%25E7%25BB%25AD%25E5%258C%2585%25E6%259C%2588%26questionJobId%3D10%26tagId%3D21003%26type%3Dvip&difficulty=undefined&judgeStatus=undefined&tags=&title=
https://leetcode.cn/problems/longest-common-prefix/description/
 */
class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     *
     * @param strs string字符串vector
     * @return string字符串
     */
    string longestCommonPrefix(vector<string>& strs) {
        // write code here
        if (strs.empty()) { return ""; }
        if (strs.size() == 1) { return strs[0]; }

        auto check = [&](int idx)->bool {
            char ch = strs[0][idx];
            for (int i = 1; i < strs.size(); ++i) {
                if (idx >= strs[i].length())
                { return false; }
                if (strs[i][idx] != ch)
                { return false; }
            }
            return true;
        };

        int i = 0;
        string result;
        while (true) {
            if (check(i)) {
                result.append(1, strs[0][i]);
                ++i;
            } else {
                break;
            }
        }

        return result;
    }
};

// 纵向比较
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        std::string result;

        if (strs.empty()) {
            return result;
        }

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
        while (is_equal(i)) {
            result.append(1, strs[0][i]);
            ++i;
        }

        return result;
    }
};

// 横向比较
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) { return ""; }
        if (strs.size() == 1) { return strs[0]; }

        auto common_prefix = [](const string& lhs, const string& rhs)->string {
            if (lhs.empty() || rhs.empty())
            { return ""; }

            string prefix;

            for (std::size_t i = 0; i < lhs.length() && i < rhs.length(); ++i) {
                if (lhs[i] == rhs[i]) {
                    prefix.append(1, lhs[i]);
                } else {
                    break;
                }
            }

            return prefix;
        };

        string result = strs[0];

        for (std::size_t i = 1; i < strs.size(); ++i) {
            result = common_prefix(result, strs[i]);
        }

        return result;
    }
};












