/**
https://leetcode.cn/problems/path-sum/description/

给你二叉树的根节点 root 和一个表示目标和的整数 targetSum 。
判断该树中是否存在 根节点到叶子节点 的路径，这条路径上所有节点值相加等于目标和 targetSum 。
如果存在，返回 true ；否则，返回 false 。

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
        bool found = false;
        traverse(root, 0, targetSum, found);
        return found;
    }

    void traverse(TreeNode* p, int sum, const int& target, bool& found) {
        if (!p || found) { return; }

        sum += p->val;
        if (sum == target && (!p->left && !p->right)) {
            found = true;
            return;
        }

        traverse(p->left, sum, target, found);
        traverse(p->right, sum, target, found);
    }
};
