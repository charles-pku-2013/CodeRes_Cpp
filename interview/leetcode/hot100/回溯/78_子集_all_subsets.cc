#if 0
https://leetcode.cn/problems/subsets/description/?envType=problem-list-v2&envId=2cktkvj
输入：nums = [1,2,3]
输出：[[],[1],[2],[1,2],[3],[1,3],[2,3],[1,2,3]]
#endif

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void subsets(std::string& s, std::string& result, int idx) {
    if (idx >= s.length()) {
        cout << result << endl;
        return;
    }

    result.push_back(s[idx]);
    subsets(s, result, idx + 1);
    result.pop_back();
    subsets(s, result, idx + 1);
}

int main() {
    // Solution sln;
    // sln.subsets({1,2,3});

    std::string s("abc");
    std::string t;
    subsets(s, t, 0);

    return 0;
}

#if 0
class Solution {
public:
    void subsets(const vector<int>& nums, vector<int>& tmp, int step, vector<vector<int>>& result) {
        if (step == nums.size()) {
            result.emplace_back(tmp);
            fmt::print("{}\n", tmp);
            return;
        }

        tmp.emplace_back(nums[step]);
        subsets(nums, tmp, step + 1, result);
        tmp.pop_back();
        subsets(nums, tmp, step + 1, result);
    }

    vector<vector<int>> subsets(const vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> tmp;
        subsets(nums, tmp, 0, result);
        return result;
    }
};
#endif

