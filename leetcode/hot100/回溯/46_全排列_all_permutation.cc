class Solution {
public:
    void backtrack(vector<vector<int>>& res, vector<int>& output, int index, int len){
        // 所有数都填完了
        if (index == len) {
            res.emplace_back(output);
            return;
        }
        for (int i = index; i < len; ++i) {
            // 动态维护数组
            swap(output[i], output[index]);
            // 继续递归填下一个数
            backtrack(res, output, index + 1, len);
            // 撤销操作
            swap(output[i], output[index]);
        }
    }

    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int> > res;
        backtrack(res, nums, 0, nums.size());
        return res;
    }
};
