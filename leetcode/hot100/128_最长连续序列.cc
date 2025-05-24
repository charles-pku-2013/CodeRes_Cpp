class Solution {
 public:
    int longestConsecutive(vector<int>& nums) {
        std::unordered_set<int> num_set(nums.begin(), nums.end());

        int max_len = 0;
        for (const int& n : num_set) {
            int cur_len = 1;
            if (num_set.count(n - 1)) {
                // 前序已经存在
                continue;
            }
            // 找不到n-1，说明n是序列起点
            for (int k = n + 1;; ++k) {
                if (num_set.count(k)) {
                    ++cur_len;
                } else {
                    break;  // 序列的终点
                }
            }
            if (cur_len > max_len) {
                max_len = cur_len;
            }
        }

        return max_len;
    }

    int longestConsecutive(vector<int>& nums) {
        std::sort(nums.begin(), nums.end());

        int curlen = 1;
        int maxlen = 0;
        for (std::size_t i = 1; i < nums.length(); ++i) {
            if (nums[i-1] == nums[i]) {
                continue;
            }
            if (nums[i] == nums[i - 1] + 1) {
                // in sequence
                ++curlen;
            } else {
                // sequence end
                maxlen = std::max(maxlen, curlen);
                curlen = 1;
            }
        }  // for i

        return maxlen;
    }
};
