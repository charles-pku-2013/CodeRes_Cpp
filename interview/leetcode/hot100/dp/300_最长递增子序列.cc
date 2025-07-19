/*
c++ -o /tmp/test test.cc -I/opt/homebrew/Cellar/fmt/11.2.0/include -L/opt/homebrew/Cellar/fmt/11.2.0/lib -lfmt -std=c++17 -g
 */
#if 0
https://leetcode.cn/problems/longest-increasing-subsequence/description/?envType=problem-list-v2&envId=2cktkvj
给你一个整数数组 nums ，找到其中最长严格递增子序列的长度。
子序列 是由数组派生而来的序列，删除（或不删除）数组中的元素而不改变其余元素的顺序。例如，[3,6,2,7] 是数组 [0,3,1,6,2,2,7] 的子序列。

示例 1：
输入：nums = [10,9,2,5,3,7,101,18]
输出：4
解释：最长递增子序列是 [2,3,7,101]，因此长度为 4 。

示例 2：
输入：nums = [0,1,0,3,2,3]
输出：4

示例 3：
输入：nums = [7,7,7,7,7,7,7]
输出：1
#endif

#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <map>
#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <fmt/ranges.h>

using namespace std;

class Solution {
public:
    int lengthOfLIS(const vector<int>& nums) {
        int n = (int)nums.size();
        if (n == 0) {
            return 0;
        }

        fmt::print("nums: {}\n", nums);

        vector<int> dp(n, 1); // 初始化为1，最长序列是我自己
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[j] < nums[i]) {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
            fmt::print("i = {}, val = {}\n", i, nums[i]);
            fmt::print("dp: {}\n", dp);
        }

        return *max_element(dp.begin(), dp.end());
    }
};

int main() {
    Solution sln;
    sln.lengthOfLIS({10,9,2,5,3,7,101,18});

    return 0;
}
