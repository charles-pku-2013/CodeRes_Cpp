/*
https://leetcode.cn/problems/top-k-frequent-words/description/
 */

class Solution {
public:
    // 用指针数组排序提高效率
    vector<string> topKFrequent(vector<string>& words, int k) {
        using Table = std::unordered_map<std::string, int>;
        Table table;

        for (auto& word : words)
        { table[word]++; }

        std::vector<const Table::value_type*> arr;
        arr.reserve(table.size());

        // NOTE map按照value排序，指针数组排序
        std::for_each(table.begin(), table.end(), [&arr](const auto& val){
            arr.emplace_back(&val);
        });

        // NOTE partial_sort 三个参数定区间
        std::partial_sort(arr.begin(), arr.begin() + k, arr.end(),
                    [](const auto* lhs, const auto* rhs)->bool {
            int diff = lhs->second - rhs->second;
            if (diff) {
                return diff > 0;
            }
            return lhs->first.compare(rhs->first) < 0;
        });

        vector<string> result;
        result.reserve(k);
        // NOTE!!! 注意避坑 时刻警惕越界
        for (int i = 0; i < k && i < arr.size(); ++i) {
            result.emplace_back(arr[i]->first);
        }

        return result;
    }

    vector<string> topKFrequent(vector<string>& words, int k) {
        std::unordered_map<std::string, int> table;
        for (auto& word : words)
        { table[word]++; }
        std::vector<std::pair<std::string, int>> arr(table.begin(), table.end());
        std::partial_sort(arr.begin(), arr.begin()+k, arr.end(),
            [](const std::pair<std::string, int>& lhs, const std::pair<std::string, int>& rhs)->bool {
                int diff = lhs.second - rhs.second;
                if (diff) { return diff > 0; }
                return lhs.first.compare(rhs.first) < 0;
            }
        );

        vector<string> result;
        for (int i = 0; i < k; ++i) {
            result.emplace_back(std::move(arr[i].first));
        }

        return result;
    }
};

class Solution {
public:
    vector<string> topKFrequent(vector<string>& words, int k) {
        std::unordered_map<std::string, int> table;
        for (auto& word : words)
        { table[word]++; }

        // using CntType = decltype(table)::value_type;  // NOTE 不可以这样用
        using CntType = std::pair<std::string, int>;
        auto cmp = [](const CntType& lhs, const CntType& rhs)->bool {
            int diff = lhs.second - rhs.second;
            if (diff) { return diff > 0; }
            return lhs.first.compare(rhs.first) < 0;
        };

        // NOTE!!! std::less<T> (默认)， top()是最大值
        // std::greater<T> top()是最小值

        std::priority_queue<CntType, std::vector<CntType>, decltype(cmp)> que(cmp);
        for (const auto& kv : table) {
            que.emplace(kv);
            if (que.size() > k)  // NOTE push first, then pop
            { que.pop(); }
        }
        // cout << que.size() << endl;

        vector<string> result;
        while (!que.empty()) {
            result.emplace_back(que.top().first);
            que.pop();
        }
        std::reverse(result.begin(), result.end());
        return result;
    }
};

