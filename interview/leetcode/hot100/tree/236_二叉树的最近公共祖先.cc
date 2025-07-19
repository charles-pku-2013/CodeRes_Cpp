/*
https://leetcode.cn/problems/lowest-common-ancestor-of-a-binary-tree/description/?envType=problem-list-v2&envId=2cktkvj
 */
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* s1, TreeNode* s2) {
        TreeNode* p = root;
        std::vector<TreeNode*> stk;
        std::vector<bool> flag;
        std::vector<TreeNode*> path_s1, path_s2;

        int found = 0;
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
                    if (q == s1) {
                        path_s1.assign(stk.rbegin(), stk.rend());
                        if (++found == 2) {
                            break;
                        }
                    } else if (q == s2) {
                        path_s2.assign(stk.rbegin(), stk.rend());
                        if (++found == 2) {
                            break;
                        }
                    }
                    stk.pop_back();
                    flag.pop_back();
                }
            }
        } // while

        std::unordered_set<TreeNode*> node_set(path_s2.begin(), path_s2.end());
        for (auto& node : path_s1) {
            if (node_set.count(node)) {
                return node;
            }
        }

        return nullptr;
    }
};
