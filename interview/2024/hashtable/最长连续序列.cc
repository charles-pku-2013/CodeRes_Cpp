/*
https://leetcode.cn/problems/longest-consecutive-sequence/
 */
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        std::unordered_set<int> num_set(nums.begin(), nums.end())

        int max_len = 0;
        for (const int& n : num_set) {  // NOTE 遍历set key若用引用必须const
            int cur_len = 1;
            if (num_set.count(n - 1))  // 已经被统计过就忽略
            { continue; }
            for (int k = n + 1; ; ++k) {
                if (num_set.count(k))
                { ++cur_len; }
                else
                { break; }
            }
            if (cur_len > max_len)
            { max_len = cur_len; }
        }

        return max_len;
    }
};
