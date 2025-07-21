/*
 * https://leetcode.cn/problems/path-sum/
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
    bool hasPathSum(TreeNode* root, int targetSum) {
        if (!root) { return false; }
        return postTraverse(root, targetSum);
    }

    // 二叉树后序遍历非递归
    bool postTraverse(TreeNode* p, int targetSum) {
        std::vector<TreeNode*> stk;
        std::vector<bool> flag;

        auto sum_path = [&](void)->int {
            int sum = 0;
            for (TreeNode *p : stk) {
                sum += p->val;
            }
            return sum;
        };

        while (p || !stk.empty()) {
            while (p) {
                stk.push_back(p);
                flag.push_back(false);
                p = p->left;
            }
            if (!stk.empty()) {
                auto* q = stk.back();
                if (flag.back() == false) {
                    p = q->right;
                    flag.back() = true;
                } else {
                    // put your logic here
                    if (!q->left && !q->right) {
                        // a leaf node
                        int sum = sum_path();
                        if (sum == targetSum)
                        { return true; }
                    }
                    stk.pop_back();
                    flag.pop_back();
                }
            }
        }

        return false;
    }
};
