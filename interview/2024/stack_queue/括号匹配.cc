/*
https://www.nowcoder.com/practice/37548e94a270412c8b9fb85643c8ccc2?tpId=196&tqId=37083&rp=1&ru=/exam/intelligent&qru=/exam/intelligent&sourceUrl=%2Fexam%2Fintelligent%3FdayCountBigMember%3D%25E8%25BF%259E%25E7%25BB%25AD%25E5%258C%2585%25E6%259C%2588%26questionJobId%3D10%26tagId%3D21003%26type%3Dvip&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 
     * @param s string字符串 
     * @return bool布尔型
     */
    bool isValid(string s) {
        // write code here
        std::vector<char> stk;
        for (char ch : s) {
            switch (ch) {
                case '(':
                case '[':
                case '{':
                    stk.push_back(ch);
                    break;
                case ')':
                    // NOTE!!! CHECK EMPTY!!!
                    if (stk.empty() || stk.back() != '(')
                    { return false; }
                    stk.pop_back();
                    break;
                case ']':
                    if (stk.empty() || stk.back() != '[')
                    { return false; }
                    stk.pop_back();
                    break;
                case '}':
                    if (stk.empty() || stk.back() != '{')
                    { return false; }
                    stk.pop_back();
                    break;
            }
        }
        return (stk.empty() ? true : false);
    }
};
