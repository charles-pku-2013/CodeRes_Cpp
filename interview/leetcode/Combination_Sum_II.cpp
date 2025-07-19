// https://leetcode.com/problems/combination-sum-ii/
/*
 * Given a collection of candidate numbers (C) and a target number (T), find all unique combinations in C where the candidate numbers sums to T.
 * 
 * Each number in C may only be used once in the combination.
 * 
 * Note:
 * All numbers (including target) will be positive integers.
 * The solution set must not contain duplicate combinations.
 * For example, given candidate set [10, 1, 2, 7, 6, 1, 5] and target 8, 
 * A solution set is: 
 * [
 *   [1, 7],
 *   [1, 2, 5],
 *   [2, 6],
 *   [1, 1, 6]
 * ]
 */
/*
    和I一样，暴搜就能过了
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
    vector<vector<int> > combinationSum2(vector<int> &candidates, int target) 
    {
        sort(candidates.begin(), candidates.end());
        vector<vector<int> > res;
        vector<int>          tmp;
        dfs(0, 0, candidates, tmp, res, target);
        return res;
    }

    void dfs(int cur, int depth, vector<int> &a, vector<int> tmp, vector<vector<int> > &res, int target)
    {
        if (target == 0) {
            res.push_back(tmp);
            return;
        } // if

        for (int i = cur; i < a.size(); ++i) {
            //两个剪枝，当前数大于目标值或当前数在该层已经被搜过要跳过
            if(target - a[i] < 0)   break;
            if(i != cur && a[i] == a[i-1])  continue;

            if(tmp.size() == depth)     
                tmp.push_back(a[i]);
            else    
                tmp[depth] = a[i];

            dfs(i + 1, depth + 1, a, tmp, res, target - a[i]);   //避免重复所以这里是i + 1
        } // for
    }
};
