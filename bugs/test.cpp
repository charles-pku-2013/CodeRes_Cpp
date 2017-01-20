#include <iostream>

using namespace std;


int main()
{
    int a = 5;

    for (int i = 0; i < 5; ++i) {
        int a = i * 2;
        cout << "a = " << a << endl;
    } // for

    cout << "a = " << a << endl;

    return 0;
}


#if 0
/*
 * 要利用名称覆盖这一特性，尽量不要重新起名。
 */

auto &cList = mit->second;
auto ret = g_WordWgtTable.insert(std::make_pair(key, WordWeightTable::mapped_type()));
ret.first->second.weight = cList.begin()->weight;
ret.first->second.count = 1;
for (auto it = cList.begin() + 1; it != cList.end(); ++it) {
    auto& word = *(boost::get<StringPtr>(it->item));
    // 只对tagSet未出现的 concur word 使用平均weight
    if (!tagSet.count(word)) {
        auto cret = g_WordWgtTable.insert(std::make_pair(word, WordWeightTable::mapped_type()));
        cret.first->second.weight += it->weight;
        ++(ret.first->second.count);
    } // if
} // for it

#endif
