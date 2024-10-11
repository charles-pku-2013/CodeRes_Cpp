/*
https://leetcode.cn/problems/removing-stars-from-a-string/?envType=daily-question&envId=2024-09-14
 */
class Solution {
public:
    string removeStars1(string s) {
        std::size_t pos = s.find('*');  // NOTE find( CharT ch, size_type pos = 0 )
                                        // find( const basic_string& str, size_type pos = 0 )
                                        // 与compare参数区别
        while (pos != string::npos) {
            if (pos == 0) {
                s.erase(pos, 1);        // NOTE string& erase( size_type index = 0, size_type count = npos );
                                        // iterator erase( iterator position );
                                        // iterator erase( iterator first, iterator last );
            } else {
                s.erase(pos - 1, 2);
            }
            pos = s.find('*');
        }
        return s;
    }

    string removeStars(string s) {
        string result;
        result.reserve(s.length());
        for (char ch : s) {
            if (ch == '*') {
                if (!result.empty())
                { result.pop_back(); }
            } else {
                result.push_back(ch);
            }
        }
        return result;
    }
};
