/*
https://leetcode.cn/problems/daily-temperatures/description/?envType=problem-list-v2&envId=2cktkvj
 */
#include <iostream>
#include <vector>

// 给定一个整数数组 temperatures ，表示每天的温度，返回一个数组 answer ，其中 answer[i] 是指对于第 i
// 天，下一个更高温度出现在几天后。如果气温在这之后都不会升高，请在该位置用 0 来代替。
// 示例 1:
// 输入: temperatures = [73,74,75,71,69,72,76,73]
// 输出: [1,1,4,2,1,1,0,0]
//
// 示例 2:
// 输入: temperatures = [30,40,50,60]
// 输出: [1,1,1,0]
//
// 示例 3:
// 输入: temperatures = [30,60,90]
// 输出: [1,1,0]
#include <fmt/base.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <chrono>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

class Solution {
 public:
    vector<int> dailyTemperatures(const vector<int>& temperatures) {
        fmt::print("temp: {}\n", temperatures);

        int         n = temperatures.size();
        vector<int> ans(n);
        vector<int> s;

        for (int i = 0; i < n; ++i) {
            while (!s.empty() && temperatures[i] > temperatures[s.back()]) {
                // 当前温度比栈顶元素高，就一直出栈并更新
                int previousIndex = s.back();
                ans[previousIndex] = i - previousIndex;
                s.pop_back();
            }
            s.push_back(i);  // 无条件入栈

            fmt::print("i: {}\n", i);
            fmt::print("ans: {}\n", ans);
            fmt::print("stack: {}\n", s);
        }

        return ans;
    }
};

int main() {
    Solution sln;
    auto     ans = sln.dailyTemperatures({73, 74, 75, 71, 69, 72, 76, 73});
    fmt::print("Result: {}\n", ans);

    return 0;
}
