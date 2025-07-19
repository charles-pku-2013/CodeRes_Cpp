/**
https://leetcode.cn/problems/path-sum-ii/description/
给你二叉树的根节点 root 和一个整数目标和 targetSum ，找出所有
从根节点到叶子节点 路径总和等于给定目标和的路径。

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
        vector<vector<int>> result;

        if (!root) { return result; }

        TreeNode *p = root;
        std::vector<TreeNode*>  stk;
        std::vector<bool>       flag;

        auto check_path = [&] {
            std::vector<int> num_path;
            num_path.reserve(stk.size());

            int sum = 0;
            std::for_each(stk.begin(), stk.end(), [&](TreeNode *p) {
                sum += p->val;
                num_path.emplace_back(p->val);
            });

            if (sum == targetSum) {
                result.emplace_back(std::move(num_path));
            }
        };

        while (p || !stk.empty()) {
            while (p) {
                stk.emplace_back(p);
                flag.emplace_back(false);
                p = p->left;
            }

            if (!stk.empty()) {
                auto& q = stk.back();
                if (!flag.back()) {
                    // back from left
                    flag.back() = true;
                    p = q->right;
                } else {
                    // back from right
                    if (!q->left && !q->right) {
                        check_path();
                    }
                    stk.pop_back();
                    flag.pop_back();
                }
            }
        }  // while

        return result;
    }
};
