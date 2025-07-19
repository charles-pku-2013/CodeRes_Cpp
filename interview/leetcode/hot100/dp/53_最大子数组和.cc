/*
 * https://leetcode.cn/problems/maximum-subarray/description/?envType=problem-list-v2&envId=2cktkvj
 */
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int pre = 0, maxAns = nums[0];
        for (const auto &x: nums) {
            pre = max(pre + x, x);
            maxAns = max(maxAns, pre);
        }
        return maxAns;
    }
};
