#include <iostream>
#include <string>
#include <vector>

using namespace std;

void all_permutation(std::string& s, int idx) {
    if (idx >= s.length()) {
        cout << s << endl;
        return;
    }

    // NOTE!!! i 不能从 idx + 1 开始
    for (auto i = idx; i < s.length(); ++i) {
        std::swap(s[i], s[idx]);
        all_permutation(s, idx + 1);
        std::swap(s[idx], s[i]);
    }
}

int main() {
    std::string s("abc");

    all_permutation(s, 0);

    return 0;
}

class Solution {
public:
    void backtrack(vector<vector<int>>& res, vector<int>& seq, int index, int len){
        // 所有数都填完了
        if (index == len) {
            res.emplace_back(seq);
            return;
        }
        for (int i = index; i < len; ++i) {
            // 动态维护数组
            swap(seq[i], seq[index]);
            // 继续递归填下一个数
            backtrack(res, seq, index + 1, len);
            // 撤销操作
            swap(seq[i], seq[index]);
        }
    }

    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int> > res;
        backtrack(res, nums, 0, nums.size());
        return res;
    }
};

