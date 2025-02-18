#include <iostream>
#include <deque>

struct BiTreeNode {
    int data;
    BiTreeNode *left = nullptr;
    BiTreeNode *right = nullptr;
};

bool IsCompleteTree(BiTreeNode* root) {
    if (!root) { return true; }
    if (!root->left && root->right)
    { return false; }
    return IsCompleteTree(root->left) && IsCompleteTree(root->right);
}

bool IsCompleteTree(BiTreeNode* p) {
    if (!p) { return true; }
    std::deque<BiTreeNode*> que;
    bool flag = false;
    que.push_back(p);
    while (!que.empty()) {
        auto* q = que.front();
        if (!q->left && q->right)
        { return false; }
        if (q->left && !q->right) {
            if (!flag) { flag = true; }
            else { return false; }
        }
        if (q->left) {
            que.push_back(q->left);
        }
        if (q->right) {
            que.push_back(q->right);
        }
        que.pop_front();
    }
}
