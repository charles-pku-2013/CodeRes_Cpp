#if 0
https://leetcode.cn/problems/letter-combinations-of-a-phone-number/description/?envType=problem-list-v2&envId=2cktkvj
给定一个仅包含数字 2-9 的字符串，返回所有它能表示的字母组合。答案可以按 任意顺序 返回。
给出数字到字母的映射如下（与电话按键相同）。注意 1 不对应任何字母。

示例 1：
输入：digits = "23"
输出：["ad","ae","af","bd","be","bf","cd","ce","cf"]

示例 2：
输入：digits = ""
输出：[]

示例 3：
输入：digits = "2"
输出：["a","b","c"]

回溯法
#endif

class Solution {
public:
    vector<string> letterCombinations(string digits) {
        vector<string> combinations;
        if (digits.empty()) {
            return combinations;
        }

        string combination;
        backtrack(combinations, digits, 0, combination);
        return combinations;
    }

    void backtrack(vector<string>& combinations, const string& digits, int index, string& combination) {
        if (index == digits.length()) {
            combinations.push_back(combination);
            return;
        }

        char digit = digits[index];
        const string& letters = phoneMap.at(digit);
        for (const char& letter: letters) {
            combination.push_back(letter);
            backtrack(combinations, digits, index + 1, combination);
            combination.pop_back();
        }
    }

private:
    unordered_map<char, string> phoneMap{
                                    {'2', "abc"},
                                    {'3', "def"},
                                    {'4', "ghi"},
                                    {'5', "jkl"},
                                    {'6', "mno"},
                                    {'7', "pqrs"},
                                    {'8', "tuv"},
                                    {'9', "wxyz"}
                                };
};

