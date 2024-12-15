#include <unordered_map>
#if 0
给定一个只包括 '('，')'，'{'，'}'，'['，']' 的字符串 s ，判断字符串是否有效。
有效字符串需满足：

左括号必须用相同类型的右括号闭合。
左括号必须以正确的顺序闭合。
每个右括号都有一个对应的相同类型的左括号。
#endif

class Solution {
public:
    // "({})"
    bool isValid(string s) {
        if (s.empty()) { return true; }
        std::vector<char> stk;
        for (char ch : s) {
            switch (ch) {
                case '(':
                    stk.push_back(ch);
                    break;
                case ')':
                    if (stk.empty() || stk.back() != '(') {
                        return false;
                    }
                    stk.pop_back();
                    break;
                case '[':
                    stk.push_back(ch);
                    break;
                case ']':
                    if (stk.empty() || stk.back() != '[') {
                        return false;
                    }
                    stk.pop_back();
                    break;
                case '{':
                    stk.push_back(ch);
                    break;
                case '}':
                    if (stk.empty() || stk.back() != '{') {
                        return false;
                    }
                    stk.pop_back();
                    break;
            }
            return true;
        }
    }
};

windows电脑管家
