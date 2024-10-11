/*
https://leetcode.cn/problems/path-sum-ii/
 */
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        using ResultType = vector<vector<int>>;
        ResultType result;
        if (!root) { return result; }

        std::vector<TreeNode*> stk;
        std::vector<bool> flag;
        TreeNode *p = root;

        auto get_sum_path = [&] {
            std::vector<int> path;
            int sum = 0;
            for (auto *p : stk) {
                sum += p->val;
                path.emplace_back(p->val);
            }
            if (sum == targetSum) {
                result.emplace_back(std::move(path));
            }
        };

        while (p || !stk.empty()) {
            while (p) {
                stk.emplace_back(p);
                flag.emplace_back(false);
                p = p->left;
            }
            if (!stk.empty()) {
                auto* q = stk.back();
                if (!flag.back()) {
                    flag.back() = true;
                    p = q->right;
                } else {
                    if (!q->left && !q->right) {
                        get_sum_path();
                    }
                    stk.pop_back();
                    flag.pop_back();
                }
            }
        } // while

        return result;
    }
};










