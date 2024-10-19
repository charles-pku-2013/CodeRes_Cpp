#include <iostream>
#include <sstream>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <list>

using namespace std;

template<typename Key, typename Value>
class LRUCache {
 public:
    struct Data {
        Data(const Key& _key, const Value& _val)
            : key(_key), value(_val) {}
        Key key;
        Value value;
    };

    using DataPtr = std::shared_ptr<Data>;

    using DataList = std::list<DataPtr>;
    using DataTable = std::unordered_map<Key, typename DataList::iterator>;

 public:
    explicit LRUCache(std::size_t c) : capacity_(c) {}

    void put(const Key& key, const Value& val) {
        auto it = data_table_.find(key);
        if (it != data_table_.end()) {
            // update
            auto list_it = it->second;
            *list_it = std::make_shared<Data>(key, val);
        } else {
            // insert new
            if (data_list_.size() >= capacity_) {
                // remove lru
                const Key& rm_key = data_list_.back()->key;
                data_table_.erase(rm_key);
                data_list_.pop_back();
            }
            data_list_.push_front(std::make_shared<Data>(key, val));
            data_table_[key] = data_list_.begin();  // NOTE 区分front begin
        }
    }

    Value get(const Key& key) {
        auto it = data_table_.find(key);
        if (it == data_table_.end())
        { return Value(-1); }
        // Move list item to fron
        auto list_it = it->second;
        DataPtr data = *list_it;
        data_list_.erase(list_it);
        data_list_.push_front(data);
        // update table
        it->second = data_list_.begin();
        return data_list_.front()->value;
    }

 private:
    std::size_t capacity_;
    DataList    data_list_;
    DataTable   data_table_;
};


int main() {
    std::size_t capacity = 0;
    char op;
    int key, value;

    cin >> capacity;
    LRUCache<int, int> cache(capacity);
    std::string line;
    while (getline(cin, line)) {
        std::stringstream ss(line);
        ss >> op;
        if (op == 'p') {
            ss >> key >> value;
            cache.put(key, value);
        } else if (op == 'g') {
            ss >> key;
            cout << cache.get(key) << endl;
        }
    }

    return 0;
}

