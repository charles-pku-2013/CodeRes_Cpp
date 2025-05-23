/*
https://leetcode.cn/problems/intersection-of-two-linked-lists/?envType=problem-list-v2&envId=2cktkvj
 */
#include <iostream>
#include <thread>

using namespace std;
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if (!headA || !headB) {
            return nullptr;
        }

        ListNode *pA = headA, *pB = headB;

        while (pA != pB) {
            pA = (pA ? pA->next : headB);
            pB = (pB ? pB->next : headA);
        }

        return pA;
    }
};
