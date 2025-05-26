#if 0
https://leetcode.cn/problems/implement-trie-prefix-tree/?envType=problem-list-v2&envId=2cktkvj
Trie（发音类似 "try"）或者说 前缀树 是一种树形数据结构，用于高效地存储和检索字符串数据集中的键。
这一数据结构有相当多的应用情景，例如自动补全和拼写检查。

请你实现 Trie 类：

Trie() 初始化前缀树对象。
void insert(String word) 向前缀树中插入字符串 word 。
boolean search(String word) 如果字符串 word 在前缀树中，返回 true（即，在检索之前已经插入）；否则，返回 false 。
boolean startsWith(String prefix) 如果之前已经插入的字符串 word 的前缀之一为 prefix ，返回 true ；否则，返回 false 。
#endif

class Trie {
 public:
    Trie() = default;

    Trie(char ch) : data(ch) {}

    void insert(string word) {
        Trie* node = this;
        for (char ch : word) {
            node = node->addChild(new Trie(ch));
        }
        node->isEnd = true;
    }

    bool search(string word) {
        Trie* node = this;
        for (char ch : word) {
            node = node->getChild(ch);
            if (!node) {
                return false;
            }
        }
        return node && node->isEnd();
    }

    bool startsWith(string prefix) {
        Trie* node = this;
        for (char ch : prefix) {
            node = node->getChild(ch);
            if (!node) {
                return false;
            }
        }
        return true;
    }

    Trie* addChild(Trie* node) {
        children[node->data] = node;
        return node;
    }

    Trie* getChild(char ch) {
        auto it = children.find(ch);
        return (it == children.end() ? nullptr : it->second);
    }

    bool isEnd() const {
        return isEnd;
    }

 private:
    char                            data = 0;
    bool isEnd = false;
    std::unordered_map<char, Trie*> children;
};
