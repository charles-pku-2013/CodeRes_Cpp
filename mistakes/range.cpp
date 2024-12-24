class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        auto sum_range = [&](int i, int j)->int {
            int sum = 0;
            // 建议用完全闭合区间
            for (int q = i; q <= j; ++q) {
                sum += nums[q];
            }
            cout << "sum: " << i << " " << j << " " << sum << endl;
            return sum;
        };

        int count = 0;
        // 这里用前闭后开区间
        for (int i = 0; i < nums.size(); ++i) {
            for (int j = i; j < nums.size(); ++j) {
                int sum = sum_range(i, j);
                if (sum == k) {
                    cout << i << " " << j << endl;
                    ++count;
                    // break;
                }
            } // for j
        } // for i

        return count;
    }

    int subarraySum(vector<int>& nums, int k) {
        auto sum_range = [&](int i, int j)->int {
            int sum = 0;
            // 如果这里用前闭后开区间
            for (int q = i; q < j; ++q) {
                sum += nums[q];
            }
            cout << "sum: " << i << " " << j << " " << sum << endl;
            return sum;
        };

        int count = 0;
        for (int i = 0; i < nums.size(); ++i) {
            // 那么这里必须用 <=
            for (int j = i + 1; j <= nums.size(); ++j) {
                int sum = sum_range(i, j);
                if (sum == k) {
                    cout << i << " " << j << endl;
                    ++count;
                    // break;
                }
            } // for j
        } // for i

        return count;
    }
};
