/*
https://leetcode.cn/problems/balanced-binary-tree/
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
    int level(TreeNode *p) {
        if (!p) { return 0; }
        return 1 + std::max(level(p->left), level(p->right));
    }

    bool isBalanced(TreeNode* root) {
        if (!root) { return true; }
        int l_height = level(root->left);
        int r_height = level(root->right);
        if (std::abs(l_height - r_height) >= 2)
        { return false; }
        return isBalanced(root->left) && isBalanced(root->right);
    }
};
