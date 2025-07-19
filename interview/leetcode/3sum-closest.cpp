// https://leetcode.com/problems/3sum-closest/
/*
 * Given an array S of n integers, find three integers in S such that the sum is closest to a given number, target. Return the sum of the three integers. You may assume that each input would have exactly one solution.
 * 
 *     For example, given array S = {-1 2 1 -4}, and target = 1.
 * 
 *     The sum that is closest to the target is 2. (-1 + 2 + 1 = 2).
 */
/*
    假设三个数  a <= b <= c 
    为了求a+b+c和target最接近 于是我们先排序然后O(n)枚举a
    然后对于每个a我们再O(n)枚举a右边的数
    最后总的时间复杂度O(n^2)
*/

#include <cmath>
#include <algorithm>

class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        sort(nums.begin(),nums.end());
        int res = nums[0] + nums[1] + nums[2];
        int n = nums.size();
        for (int i = 0; i < n && res != target; ++i) {            //枚举a
            if (i && nums[i] == nums[i-1]) continue;         //排除重复的解

            int lhs = i+1, rhs = n-1;                            //b和c的下标
            while (rhs > lhs) {                                   //O(n)枚举b和c
                int tmp = nums[i] + nums[lhs] + nums[rhs];
                if (abs(tmp - target) < abs(res - target))    
                    res = tmp;                                 //当前解比原来解好  替换
                if (tmp > target)   --rhs;
                else if(tmp < target)   ++lhs;
                else   break;
            } // while
        } // for

        return res;
    }
};
