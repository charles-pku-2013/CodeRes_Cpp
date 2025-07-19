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
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* n1, TreeNode* n2) {
        if (!root || !n1 || !n2) { return nullptr; }

        std::vector<TreeNode*> stk;
        std::vector<bool>      flag;
        std::vector<TreeNode*> path1, path2;

        TreeNode *p = root;
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
                    p = q->right;
                    flag.back() = true;
                } else {
                    // back from right check the node
                    if (q == n1) {
                        path1.assign(stk.begin(), stk.end());
                    } else if (q == n2) {
                        path2.assign(stk.begin(), stk.end());
                    }

                    stk.pop_back();
                    flag.pop_back();
                }
            }  // if
        }  // while

        // common prefix of paths
        int i = 0;
        for (; i < path1.size() && i < path2.size() && path1[i] == path2[i]; ++i);
        --i;

        return i < 0 ? nullptr : path1[i];
    }

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
