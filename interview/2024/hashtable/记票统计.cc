/*
https://www.nowcoder.com/practice/3350d379a5d44054b219de7af6708894?tpId=37&tqId=21317&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D2%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;

struct Candidate {
    Candidate() = default;  // NOTE required
    Candidate(const string &_name, int _no)
        : name(_name), no(_no) {}
    string name;
    int no = 0;
    int count = 0;
};

int main() {
    std::unordered_map<string, Candidate> table;
    int n, m;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        string name;
        cin >> name;
        table[name] = Candidate(name, i);
    }
    cin >> m;
    int invalid_cnt = 0;
    while (m--) {
        string name;
        cin >> name;
        auto it = table.find(name);
        if (it == table.end()) { 
            ++invalid_cnt; 
        } else {
            it->second.count++;
        }
    } // while
    vector<Candidate*> arr;
    for (auto& kv : table) {
        arr.emplace_back(&kv.second);
    }
    std::sort(arr.begin(), arr.end(),
        [](const Candidate* lhs, const Candidate* rhs)->bool {
            return lhs->no < rhs->no;
        }
    );
    for (auto* p : arr) {
        cout << p->name << " : " << p->count << endl;
    }
    cout << "Invalid : " << invalid_cnt << endl;
}
// 64 位输出请用 printf("%lld")
