class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        std::string result;

        if (strs.empty()) {
            return result;
        }

        std::size_t i = 0;

        auto is_equal = [&]()->bool {
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

        while (is_equal()) {
            result.append(1, strs[0][i]);
            ++i;
        }

        return result;
    }
};
