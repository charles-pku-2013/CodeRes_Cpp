/*
struct TreeNode {
	int val;
	struct TreeNode *left;
	struct TreeNode *right;
	TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
	}
};*/
class Solution {
public:
    vector<vector<int> > FindPath(TreeNode* root,int expectNumber) {
        m_AllPaths.clear();
        m_vecCurPath.clear();

        FindPathHelper(root, 0, expectNumber, 1);

        return m_AllPaths;
    }

    bool isLeaf(TreeNode *p)
    { return !p->left && !p->right; }

    void FindPathHelper(TreeNode *p, const int sum, const int expected, const int idx)
    {
        if (!p) return;

        m_vecCurPath.resize(idx);
        m_vecCurPath[idx-1] = p->val;
        int curSum = sum + p->val;
        if (curSum == expected && isLeaf(p)) {
            m_AllPaths.push_back(vector<int>());
            m_AllPaths.back().assign(m_vecCurPath.begin(), m_vecCurPath.begin()+idx);
        } // if

        FindPathHelper(p->left, curSum, expected, idx+1);
        FindPathHelper(p->right, curSum, expected, idx+1);
    }

    vector<vector<int> >    m_AllPaths;
    vector<int>             m_vecCurPath;
};
