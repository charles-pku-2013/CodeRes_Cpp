/*
https://www.nowcoder.com/practice/8e400fd9905747e4acc2aeed7240978b?tpId=37&tqId=21291&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D1%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

struct Student {
    Student(const string& _name, int _score, int _id)
        : name(_name), score(_score), id(_id) {}

    string name;
    int score;
    int id;
};

struct StudentCmp {
    explicit StudentCmp(int sorter)
            : sorter_(sorter) {}

    bool operator()(const Student& lhs, const Student& rhs) const {
        int diff = lhs.score - rhs.score;
        if (diff) {
            if (sorter_ == 0)
            { return diff > 0; }
            else 
            { return diff < 0; }
        }
        return lhs.id < rhs.id;
    }

    int sorter_;
};

int main() {
    int n, sorter, score;
    string name;
    vector<Student> arr;
    cin >> n >> sorter;
    for (int i = 0; i < n; ++i) {
        cin >> name >> score;
        arr.emplace_back(name, score, i);
    }

    std::sort(arr.begin(), arr.end(), StudentCmp(sorter));

    for (auto& item : arr) {
        cout << item.name << " " << item.score << endl;
    }
}







// 64 位输出请用 printf("%lld")
