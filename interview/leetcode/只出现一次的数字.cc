/*
https://leetcode.cn/problems/single-number/description/
 */
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int result = nums[0];

        for (auto i = 1; i < nums.size(); ++i) {
            result ^= nums[i];  // This is XOR operator
        }

        return result;
    }
};
