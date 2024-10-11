/*
https://www.nowcoder.com/practice/54404a78aec1435a81150f15f899417d?tpId=37&tqId=21274&rp=1&ru=/exam/oj/ta&qru=/exam/oj/ta&sourceUrl=%2Fexam%2Foj%2Fta%3Fpage%3D1%26tpId%3D37%26type%3D37&difficulty=undefined&judgeStatus=undefined&tags=&title=
 */
#include <iostream>
using namespace std;

struct ListNode {
    ListNode() = default;
    ListNode(int key) : m_nKey(key) {}

    int m_nKey = 0;
    ListNode* m_pNext = nullptr;
};

// NOTE 回溯法
ListNode* process(ListNode* p, int& k) {
    if (!p) { return nullptr; }
    ListNode *prev = process(p->m_pNext, k);
    if (--k == 0) { return p; }
    return prev;
}

// another
void process(ListNode* head, int& k) {
    if (!head || k < 0) { return; }
    process(head->m_pNext, k);
    if (--k == 0) {
        cout << head->m_nKey << endl;
    }
}

int main() {
    int n, val, k;
    ListNode *head = nullptr;
    ListNode *cur = nullptr;

    while (cin >> n) {
        while (n--) {
            cin >> val;
            ListNode* p = new ListNode(val);
            if (!cur) {
                cur = p;
            } else {
                cur->m_pNext = p;
                cur = p;
            }
            if (!head) {
                head = cur;
            }
        }
        cin >> k;
        // for (ListNode* p = head; p; p = p->m_pNext) {
        //     cout << p->m_nKey << endl;
        // }

        auto* p = process(head, k);
        if (p) {
            cout << p->m_nKey << endl;
        }
    } // n
}
// 64 位输出请用 printf("%lld")
