#include <iostream>

using namespace std;

struct ListNode {
    ListNode(int _Data) : data(_Data), next(NULL) {}

    int data;
    ListNode *next;
};


void create_list(ListNode *&p) // 🔴
{
    int data;
    cin >> data;

    // 🔴🔴 判断输入结束
    if (!cin) {
        p = NULL;
        return;
    } // if

    p = new ListNode(data);
    create_list(p->next);
}


void print_list(ListNode *p)
{
    while (p) {
        cout << p->data << " ";
        p = p->next;
    } // while
    cout << endl;
}


// NOTE!!! 🔴🔴 prev 指向新表头
void reverse_list_helper(ListNode *p, ListNode *&prev)
{
    if (!p) return;

    ListNode *q = p->next;
    p->next = prev;
    prev = p;       // 🔴
    reverse_list_helper(q, prev);   // 🔴
}

ListNode* reverse_list(ListNode *p)
{
    ListNode *newHead = NULL;   // 🔴
    reverse_list_helper(p, newHead);
    return newHead;
}


ListNode* reverse_list_norecur(ListNode *p)
{
    ListNode *prev = NULL;

    while (p) {
        ListNode *q = p->next;
        p->next = prev;
        prev = p;
        p = q;
    } // while

    return prev;
}

int main()
{
    ListNode *pl = NULL;
    create_list(pl);
    print_list(pl);
    // pl = reverse_list(pl);
    pl = reverse_list_norecur(pl);
    print_list(pl);

    return 0;
}
