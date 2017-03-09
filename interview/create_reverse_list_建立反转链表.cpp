#include <iostream>

using namespace std;

struct ListNode {
    int data;
    ListNode *next;

    ListNode(int n) : data(n), next(NULL) {}
};


void CreateList(ListNode *&p)
{
    int n;
    cin >> n;
    if (!cin) return;
    p = new ListNode(n);
    CreateList(p->next);
}

ListNode* ReverseList(ListNode *p)
{
    if (!p) return p;

    ListNode *r = NULL, *q = NULL;
    while (p) {
        q = p->next;
        p->next = r; r = p; p = q;
    } // while

    return r;
}

void PrintList(ListNode *p)
{
    while (p) {
        cout << p->data << " ";
        p = p->next;
    } // while
    cout << endl;
}

int main()
{
    // Because the console is line buffered,
    // so the input should be like this:
    // 1 2 3 4 5 ... <Enter> <Ctrl-D>
    cout << "Input a sequence of number to create list:" << endl;
    ListNode *root = NULL;
    CreateList(root);
    PrintList(root);
    root = ReverseList(root);
    cout << "List after reverse is:" << endl;
    PrintList(root);

    return 0;
}

