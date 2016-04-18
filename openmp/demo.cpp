#include <iostream>
#include <map>
#include <vector>
#include <thread>

using namespace std;

int main()
{
    typedef std::map<std::string, uint32_t> StrUIntMap;

    auto process = [](StrUIntMap::value_type &v) {
        cout << "In thread " << std::this_thread::get_id() << " " << v.first << " = " << v.second << endl;
        v.second *= 2;
    };

    auto processArr = [](int &v) {
        v *= 2;
        cout << "In thread " << std::this_thread::get_id() << " value = " << v << endl;
    };

    StrUIntMap m;
    m["hello"] = 10;
    m["world"] = 20;
    m["pku"] = 100;

    // wrong
// #pragma omp parallel for
    // for (auto it = m.begin(); it != m.end(); ++it) {
        // process(*it);
    // } // for

    // auto it = m.begin();
// #pragma omp parallel for
    // for (size_t i = 0; i < m.size(); ++i) {
        // process(*it++);
    // } // for
/*
 * In thread 140408659560384 hello = 10
 * In thread 140408632211200 hello = 10
 * In thread 140408640603904 hello = 10
 */

    vector<int> arr = {1,2,3,4,5,6};
#pragma omp parallel for
    for (size_t i = 0; i < arr.size(); ++i)
        processArr(arr[i]);
    // 不可以用 i != arr.size() 判断循环结束条件 error: invalid controlling predicate
/*
 * In thread 140565560051456 value = 6
 * In thread 140565560051456 value = 8
 * In thread 140565543266048 value = 12
 * In thread 140565551658752 value = 10
 * In thread 140565579007936 value = 2
 * In thread 140565579007936 value = 4
 */

    return 0;
}
