/**
https://leetcode.cn/problems/remove-nth-node-from-end-of-list/description/?envType=problem-list-v2&envId=2cktkvj
给你一个链表，删除链表的倒数第 n 个结点，并且返回链表的头结点。
需要找到起前一个节点即倒数n+1个节点, 需要在头部加一个伪节点处理删除链表头情况
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode fake;
        fake.next = head;

        ListNode* result = nullptr;
        ++n;
        get_reverse_n_node(&fake, n, result);

        if (result->next) {
            result->next = result->next->next;
        }

        return fake.next;
    }

    void get_reverse_n_node(ListNode *p, int& n, ListNode *&result) {
        if (result || !p) { return; }

        get_reverse_n_node(p->next, n, result);

        if (--n == 0) {
            result = p;
        }
    }
};
