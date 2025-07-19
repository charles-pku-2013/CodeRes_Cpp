// https://leetcode.com/problems/combination-sum/
/*
 * Given a set of candidate numbers (C) and a target number (T), find all unique combinations in C where the candidate numbers sums to T.
 * 
 * The same repeated number may be chosen from C unlimited number of times.
 * 
 * Note:
 * All numbers (including target) will be positive integers.
 * The solution set must not contain duplicate combinations.
 * For example, given candidate set [2, 3, 6, 7] and target 7, 
 * A solution set is: 
 * [
 *   [7],
 *   [2, 2, 3]
 * ]
 *
 * VIP
 */
/*
    暴搜就能过了
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/format.hpp>
#include <glog/logging.h>

using namespace std;


ostream& operator<< (ostream &os, const vector<int> &v)
{
    std::copy(v.begin(), v.end(), ostream_iterator<int>(os, " "));
    return os;
}

class Solution {
public:
    vector<vector<int> > combinationSum(vector<int> &candidates, int target) 
    {
        std::sort(candidates.begin(), candidates.end());
        vector<vector<int> > res;
        vector<int> tmp;
        dfs(0, 0, candidates, tmp, res, target);
        return res;
    }

    // NOTE!!! tmp不可以用引用传递
    // depth 目标路径中的下标
    void dfs(int cur, int depth, vector<int> &a, vector<int> tmp, vector<vector<int> > &res, int target)
    {
        DLOG(INFO) << boost::format("dfs(%d, %d, %d)") % cur % depth % target;

        if (target == 0) {      // 找到一个
            DLOG(INFO) << "found a path: " << tmp;
            res.push_back(tmp);
            return;
        } // if

        for (int i = cur; i < a.size(); ++i) {
            //两个剪枝，当前数大于目标值或当前数在该层已经被搜过要跳过
            if (target - a[i] < 0)   break;
            if (i != cur && a[i] == a[i-1])  continue;

            if (tmp.size() == depth)     
                tmp.push_back(a[i]);
            else    
                tmp[depth] = a[i];

            DLOG(INFO) << boost::format("Set tmp[%d] = %d") % depth % a[i];

            dfs(i, depth + 1, a, tmp, res, target - a[i]);
        } // for
    }
};


int main(int argc, char **argv)
{
    Solution su;

    vector<int> v{2,3,6,7};

    auto ret = su.combinationSum(v, 7);

    cout << "Result:" << endl;
    for (auto &v : ret)
        cout << v << endl;

    return 0;
}


