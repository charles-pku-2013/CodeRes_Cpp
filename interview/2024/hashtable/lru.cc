/*
https://leetcode.cn/problems/lru-cache/
 */
class LRUCache {
 public:
    struct Data {
        Data() = default;

        Data(int k, int v) : key(k), value(v) {}

        int key;
        int value;
    };

    using DataList = std::list<Data>;
    using DataTable = std::unordered_map<int, DataList::iterator>;

    LRUCache(int capacity) {
        capacity_ = capacity;
    }

    int get(int key) {
        // find item in table
        auto it = data_table_.find(key);
        if (it == data_table_.end()) {
            return -1;
        }
        auto old_list_it = it->second;

        // relocate the item in list
        data_list_.emplace_front(std::move(*old_list_it));
        data_list_.erase(old_list_it);

        // update the table item
        it->second = data_list_.begin();

        return data_list_.front().value;
    }

    void put(int key, int value) {
        auto it = data_table_.find(key);
        if (it == data_table_.end()) {
            // new insersion in list
            data_list_.emplace_front(key, value);
            data_table_[key] = data_list_.begin();
            if (data_table_.size() > capacity_) {
                data_table_.erase(data_list_.back().key);
                data_list_.pop_back();
            }
        } else {
            // update
            data_list_.emplace_front(key, value);
            data_list_.erase(it->second);
            it->second = data_list_.begin();
        }
    }

    DataList  data_list_;
    DataTable data_table_;
    int       capacity_ = 0;
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */
