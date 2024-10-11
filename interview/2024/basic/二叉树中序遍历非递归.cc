/*
https://leetcode.cn/problems/binary-tree-inorder-traversal/
 */

class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        if (!root) { return result; }
        vector<TreeNode*> stack;
        TreeNode *p = root;
        do {
            while (p) {
                stack.push_back(p);
                p = p->left;
            }
            if (!stack.empty()) {
                p = stack.back();
                stack.pop_back();
                cout << p->val << endl;
                p = p->right;
            }
        } while (p || !stack.empty());

        return result;
    }
};
