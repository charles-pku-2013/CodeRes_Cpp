#if 0
https://leetcode.cn/problems/subarray-sum-equals-k/description/?envType=problem-list-v2&envId=2cktkvj
给你一个整数数组 nums 和一个整数 k ，请你统计并返回 该数组中和为 k 的子数组的个数 。
子数组是数组中元素的连续非空序列。

示例 1：
输入：nums = [1,1,1], k = 2
输出：2

示例 2：
输入：nums = [1,2,3], k = 3
输出：2
#endif

class Solution {
 public:
    int subarraySum(vector<int>& nums, int k) {
        unordered_map<int, int> mp;
        mp[0] = 1;  // key: 局部sum, value: 计数
        int count = 0, sum = 0;
        for (auto& x : nums) {
            sum += x;    // sum sum of all
            if (auto it = mp.find(sum - k); it != mp.end()) {
                count += it->second;
            }
            ++mp[sum];
        }
        return count;
    }
};
