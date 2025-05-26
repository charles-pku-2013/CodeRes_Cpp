#if 0
https://leetcode.cn/problems/kth-largest-element-in-an-array/description/?envType=problem-list-v2&envId=2cktkvj
给定整数数组 nums 和整数 k，请返回数组中第 k 个最大的元素。
请注意，你需要找的是数组排序后的第 k 个最大的元素，而不是第 k 个不同的元素。
你必须设计并实现时间复杂度为 O(n) 的算法解决此问题
示例 1:

输入: [3,2,1,5,6,4], k = 2
输出: 5
示例 2:

输入: [3,2,3,1,2,4,5,5,6], k = 4
输出: 4
 

提示：

1 <= k <= nums.length <= 105
-104 <= nums[i] <= 104
#endif

class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        --k;

        std::make_heap(nums.begin(), nums.end());

        for (int i = 0; i < k; ++i) {
            std::pop_heap(nums.begin(), nums.end());
            nums.pop_back();
        }

        return *(nums.begin());
    }
};

class Solution {
 public:
    int findKthLargest(vector<int>& nums, int k) {
        // 小根堆中存储数组中k个最大的元素
        priority_queue<int, vector<int>, greater<int>> heap;
        int                                            n = nums.size();
        for (int i = 0; i < n; ++i) {
            // 堆的大小小于k，向堆内添加一个元素
            if (heap.size() < k) {
                heap.push(nums[i]);
            }
            // 堆的大小大于等于k
            // 如果当前元素比堆内所有元素都小，就加入堆，反之，弹出堆内最小的那个元素后加入
            else {
                if (nums[i] > heap.top()) {
                    heap.pop();
                    heap.push(nums[i]);
                }
            }
        }
        return heap.top();
    }
};
