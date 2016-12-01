// https://leetcode.com/problems/best-time-to-buy-and-sell-stock/
/*
 * Say you have an array for which the ith element is the price of a given stock on day i.
 * 
 * If you were only permitted to complete at most one transaction (ie, buy one and sell one share of the stock), design an algorithm to find the maximum profit.
 * 
 * Example 1:
 * Input: [7, 1, 5, 3, 6, 4]
 * Output: 5
 * 
 * max. difference = 6-1 = 5 (not 7-1 = 6, as selling price needs to be larger than buying price)
 * Example 2:
 * Input: [7, 6, 4, 3, 1]
 * Output: 0
 * 
 * In this case, no transaction is done, i.e. max profit = 0.
 */
/*
    因为只能进行一次交易，所以对于每个价格，我们只要维护它前面价格中的最小值即可
    答案就是所有当前价格减去它前面价格最小值后的最大差值
    time O(n), space O(1)
*/


#if 0
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int minPrice = std::numeric_limits<int>::max();
        int result = 0;
        for (int i = 0; i < prices.size(); ++i) {
            result = std::max(result, prices[i] - minPrice);
            //维护一个最小值
            minPrice = std::min(minPrice, prices[i]);
        } // for
        return result;
    }
};
#endif

class Solution {
public:
    int maxProfit(vector<int>& prices) 
    {
        int maxProfit = 0;
        int minPrice = prices[0];

        for (size_t i = 1; i < prices.size(); ++i) {
            int profit = prices[i] - minPrice;
            if (profit > maxProfit) maxProfit = profit;
            if (prices[i] < minPrice) minPrice = prices[i];
        } // for

        return maxProfit;
    }
};
