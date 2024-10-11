/*
https://www.nowcoder.com/practice/a69242b39baf45dea217815c7dedb52b?tpId=295&tqId=2288088&ru=/exam/intelligent&qru=/ta/format-top101/question-ranking&sourceUrl=%2Fexam%2Fintelligent
 */
/**
 * struct TreeNode {
 *	int val;
 *	struct TreeNode *left;
 *	struct TreeNode *right;
 *	TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 * };
 */
class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 
     * @param root TreeNode类 
     * @return bool布尔型
     */
    bool isValidBST(TreeNode* root) {
        std::vector<int> seq;
        traverse(root, seq);
        if (seq.size() <= 1)
        { return true; }
        return std::is_sorted(seq.begin(), seq.end());  // NOTE
    }

    void traverse(TreeNode *p, std::vector<int>& seq) {
        if (p) {
            traverse(p->left, seq);
            seq.emplace_back(p->val);
            traverse(p->right, seq);
        }
    }
};







