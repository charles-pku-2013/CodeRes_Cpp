/*
https://leetcode.cn/problems/binary-tree-level-order-traversal/
 */
#include <iostream>
#include <vector>
#include <deque>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:
    vector<vector<int>> levelOrder1(TreeNode* root) {
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

    // USE THIS, In function class
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;

        if (!root) {
            return result;
        }

        struct Item {
            TreeNode* node = nullptr;
            int level = 0;
        };

        // NOTE 初始化列表的使用方法
        std::deque<Item> que;
        que.emplace_back(Item{root, 0});
        while (!que.empty()) {
            auto& item = que.front();
            int level = item.level;
            auto* p = item.node;
            if (result.size() < level + 1) {
                result.resize(level + 1);
            }
            result[level].emplace_back(p->val);
            que.pop_front();

            if (p->left) {
                que.emplace_back(Item{p->left, level + 1});
            }
            if (p->right) {
                que.emplace_back(Item{p->right, level + 1});
            }
        }

        return result;
    }
};

int main() {
    TreeNode *root = new TreeNode(3);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    auto *p = root->right;
    p->left = new TreeNode(15);
    p->right = new TreeNode(7);

    Solution s;
    s.levelOrder(root);

    return 0;
}
