#include <iostream>
#include <unordered_map>
#include <list>

using namespace std;

class LRUCache {
private:
    int capacity;
    // 双向链表，存储 {key, value}。必须存 key，因为淘汰尾部时需要用它去哈希表里删对应项
    list<pair<int, int>> cache_list;
    // 哈希表，存储 key -> 链表节点迭代器的映射
    unordered_map<int, list<pair<int, int>>::iterator> cache_map;

public:
    LRUCache(int capacity) : capacity(capacity) {}

    int get(int key) {
        // 如果找不到，返回 -1
        if (cache_map.find(key) == cache_map.end()) {
            return -1;
        }
        
        // 关键步骤：使用 splice 将访问的节点 O(1) 转移到链表头部
        // 参数依次为：目标位置，来源链表，要移动的节点迭代器
        cache_list.splice(cache_list.begin(), cache_list, cache_map[key]);
        
        return cache_map[key]->second;
    }

    void put(int key, int value) {
        auto it = cache_map.find(key);
        
        if (it != cache_map.end()) {
            // 如果 key 已存在：更新值，并将节点移到头部
            it->second->second = value;
            cache_list.splice(cache_list.begin(), cache_list, it->second);
        } else {
            // 如果 key 不存在：
            // 1. 如果容量已满，先淘汰尾部节点
            if (cache_list.size() == capacity) {
                int key_to_delete = cache_list.back().first;
                cache_list.pop_back();
                cache_map.erase(key_to_delete);
            }
            
            // 2. 将新节点插入头部，并记录其迭代器到哈希表中
            cache_list.emplace_front(key, value);
            cache_map[key] = cache_list.begin();
        }
    }
};

// 测试代码
int main() {
    LRUCache lru(2);
    lru.put(1, 1);
    lru.put(2, 2);
    cout << "get(1): " << lru.get(1) << " (预期为 1)" << endl;
    lru.put(3, 3); // 淘汰 2
    cout << "get(2): " << lru.get(2) << " (预期为 -1)" << endl;
    lru.put(4, 4); // 淘汰 1
    cout << "get(1): " << lru.get(1) << " (预期为 -1)" << endl;
    cout << "get(3): " << lru.get(3) << " (预期为 3)" << endl;
    cout << "get(4): " << lru.get(4) << " (预期为 4)" << endl;
    return 0;
}
