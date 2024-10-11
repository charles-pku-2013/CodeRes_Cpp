/*
https://leetcode.cn/problems/binary-tree-level-order-traversal/
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
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) { return result; }
        std::deque<TreeNode*> que;
        std::deque<int> level_cnt;
        que.push_back(root);
        int level = 0;
        level_cnt.push_back(level);
        result.emplace_back();

        while (!que.empty()) {
            TreeNode* p = que.front();
            level = level_cnt.front();
            que.pop_front();
            level_cnt.pop_front();
            if (result.size() < level + 1)
            { result.resize(level + 1); }
            result[level].emplace_back(p->val);
            if (p->left) {
                que.push_back(p->left);
                level_cnt.push_back(level + 1);
            }
            if (p->right) {
                que.push_back(p->right);
                level_cnt.push_back(level + 1);
            }
        }
        return result;
    }
};
