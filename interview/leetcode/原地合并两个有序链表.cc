/**
https://leetcode.cn/problems/merge-two-sorted-lists/description/
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
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode head;
        ListNode* prev = &head;

        while (l1 && l2) {
            if (l1->val < l2->val) {
                prev->next = l1;
                l1 = l1->next;
            } else {
                prev->next = l2;
                l2 = l2->next;
            }
            prev = prev->next;
        }

        // 合并后 l1 和 l2 最多只有一个还未被合并完，我们直接将链表末尾指向未合并完的链表即可
        prev->next = l1 ? l1 : l2;

        return head.next;
    }

    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (!l1) { return l2; }
        if (!l2) { return l1; }

        if (l1->val < l2->val) {
            l1->next = mergeTwoLists(l1->next, l2);
            return l1;
        } else {
            l2->next = mergeTwoLists(l1, l2->next);
            return l2;
        }
    }

    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        if (!list1) { return list2; }
        if (!list2) { return list1; }

        ListNode* it1 = list1;
        ListNode* it2 = list2;
        ListNode* result = nullptr;
        ListNode* p = nullptr;

        while (it1 && it2) {
            if (it1->val < it2->val) {
                if (p) {
                    p->next = it1;
                    p = it1;
                } else {
                    result = p = it1;
                }
                it1 = it1->next;
            } else {
                if (p) {
                    p->next = it2;
                    p = it2;
                } else {
                    result = p = it2;
                }
                it2 = it2->next;
            }
        }

        if (it1) {
            p->next = it1;
        } else {
            p->next = it2;
        }

        return result;
    }
};
