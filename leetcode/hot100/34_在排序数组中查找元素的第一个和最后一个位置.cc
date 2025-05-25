#if 0
https://leetcode.cn/problems/find-first-and-last-position-of-element-in-sorted-array/description/?envType=problem-list-v2&envId=2cktkvj
给你一个按照非递减顺序排列的整数数组 nums，和一个目标值 target。请你找出给定目标值在数组中的开始位置和结束位置。
如果数组中不存在目标值 target，返回 [-1, -1]。
你必须设计并实现时间复杂度为 O(log n) 的算法解决此问题。

示例 1：
输入：nums = [5,7,7,8,8,10], target = 8
输出：[3,4]

示例 2：
输入：nums = [5,7,7,8,8,10], target = 6
输出：[-1,-1]

示例 3：
输入：nums = [], target = 0
输出：[-1,-1]
#endif

class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        auto pos = std::equal_range(nums.begin(), nums.end(), target);
        auto it1 = pos.first;
        auto it2 = pos.second;

        std::vector<int> ret(2, -1);

        // NOTE equal_range 返回结果 first == second 表示不存在，而不是 == end()
        if (it1 == it2) {
            return ret;
        }

        ret[0] = std::distance(nums.begin(), it1);
        ret[1] = std::distance(nums.begin(), --it2);

        return ret;
    }
};
