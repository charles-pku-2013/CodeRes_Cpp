/*
https://www.nowcoder.com/practice/11ae12e8c6fe48f883cad618c2e81475?tpId=196&tqId=37176&rp=1&ru=/exam/intelligent&qru=/exam/intelligent&sourceUrl=%2Fexam%2Fintelligent%3FdayCountBigMember%3D%25E8%25BF%259E%25E7%25BB%25AD%25E5%258C%2585%25E6%259C%2588%26questionJobId%3D10%26tagId%3D21003%26type%3Dvip&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 计算两个数之和
     * @param s string字符串 表示第一个整数
     * @param t string字符串 表示第二个整数
     * @return string字符串
     */
    string solve(string s, string t) {
        // write code here
        if (s.empty()) { return t; }
        if (t.empty()) { return s; }

        std::reverse(s.begin(), s.end());
        std::reverse(t.begin(), t.end());
        int len = std::max(s.length(), t.length());
        s.resize(len, '0');
        t.resize(len, '0');

        std::string r;
        r.resize(len, '0');
        int addone = 0;
        for (int i = 0; i < len; ++i) {
            int sum = c2n(s[i]) + c2n(t[i]) + addone;
            if (sum >= 10) {
                addone = 1;
                sum %= 10;
            } else {
                addone = 0;
            }
            r[i] = n2c(sum);
        }
        if (addone)
        { r.push_back('1'); }
        
        std::reverse(r.begin(), r.end());
        return r;
    }

    char n2c(int n)
    { return '0' + n; }
    int c2n(char c)
    { return c - '0'; }
};










