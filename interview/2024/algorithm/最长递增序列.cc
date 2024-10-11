/*
https://www.nowcoder.com/practice/24e6243b9f0446b081b1d6d32f2aa3aa?tpId=37&tqId=21326&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D2%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 截止到第i个元素为基准的最长序列是前i-1的最长序列加上他本身（如果他比之前的大的话）
int compute(vector<int>& nums) {
    vector<int> dp(nums.size(), 1); // 全部初始化为1

    for (int i = 1; i < nums.size(); ++i) {
        for (int j = 0; j < i; ++j) {
            if (nums[i] > nums[j]) {
                dp[i] = std::max(dp[i], dp[j] + 1);
            }
        }
    } // for i

    auto it = std::max_element(dp.begin(), dp.end());  // NOTE 返回迭代器
    return *it;
}

int main() {
    int n, k;
    vector<int> arr;
    cin >> n;
    while (n--) {
        cin >> k;
        arr.emplace_back(k);
    }
    int result = compute(arr);
    cout << result << endl;
}
// 64 位输出请用 printf("%lld")
