/*
https://leetcode.cn/problems/reverse-words-in-a-string-ii/
 */
class Solution {
public:
    void reverseWords(vector<char>& s) {
        string str(s.begin(), s.end());
        vector<string> result;

        // NOTE 单词分割
        auto w_beg = str.find_first_not_of(" ");
        while (w_beg != string::npos) {
            auto w_end = str.find_first_of(" ", w_beg);  // w_end可能是npos
            result.emplace_back(str.substr(w_beg, w_end - w_beg));
            w_beg = str.find_first_not_of(" ", w_end);
        }

        std::reverse(result.begin(), result.end());
        stringstream ss;
        // debug this way
        std::copy(result.begin(), result.end(), std::ostream_iterator<string>(ss, " "));
        string ret = ss.str();
        ret.pop_back();
        s.assign(ret.begin(), ret.end());
    }
};
