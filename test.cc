class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        // std::sort(nums.begin(), nums.end());
        // auto it = std::lower_bound(nums.begin(), nums.end(), k);

        auto sum_range = [&](int i, int j)->int {
            int sum = 0;
            for (int q = i; q < j; ++q) {
                sum += nums[q];
            }
            return sum;
        };

        int count = 0;
        for (int i = 0; i < nums.size(); ++i) {
            for (int j = i + 1; j < nums.size(); ++j) {
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
