class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if (nums.size() <= 1) { return nums.size(); }

        int k = 0;
        for (int i = 1; i < nums.size(); ++i) {
            if (nums[i] != nums[k]) {
                nums[k++] = nums[i];
            }
        }

        nums.resize(k);
        return k;
    }
};
